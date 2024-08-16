#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <wn.h>
#if CONFIG_ACL
#include <sys/acl.h>
#endif
#if CONFIG_SELINUX
#include <selinux/selinux.h>
#endif

#include "text.h"
#include "text-internal.h"
#include "text-util.h"
#include "util.h"

struct TextSave {                  /* used to hold context between text_save_{begin,commit} calls */
	Text *txt;                 /* text to operate on */
	char *filename;            /* filename to save to as given to text_save_begin */
	char *tmpname;             /* temporary name used for atomic rename(2) */
	int fd;                    /* file descriptor to write data to using text_save_write */
	enum TextSaveMethod type;  /* method used to save file */
};

/* Allocate blocks holding the actual file content in chunks of size: */
#ifndef BLOCK_SIZE
#define BLOCK_SIZE (1 << 20)
#endif

/* allocate a new block of MAX(size, BLOCK_SIZE) bytes */
Block *block_alloc(size_t size) {
	Block *blk = calloc(1, sizeof *blk);
	if (!blk)
		return NULL;
	if (BLOCK_SIZE > size)
		size = BLOCK_SIZE;
	if (!(blk->data = malloc(size))) {
		free(blk);
		return NULL;
	}
	blk->size = size;
	return blk;
}

Block *block_read(size_t size, int fd) {
	Block *blk = block_alloc(size);
	if (!blk)
		return NULL;
	char *data = blk->data;
	size_t rem = size;
	while (rem > 0) {
		ssize_t len = read(fd, data, rem);
		if (len == -1) {
			block_free(blk);
			return NULL;
		} else if (len == 0) {
			break;
		} else {
			data += len;
			rem -= len;
		}
	}
	blk->len = size - rem;
	return blk;
}

Block *block_load(const char *filename, enum TextLoadMethod method, struct stat *info) {
	Block *block = NULL;
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
		goto out;
	if (fstat(fd, info) == -1)
		goto out;
	if (!S_ISREG(info->st_mode)) {
		errno = S_ISDIR(info->st_mode) ? EISDIR : ENOTSUP;
		goto out;
	}

	// XXX: use lseek(fd, 0, SEEK_END); instead?
	size_t size = info->st_size;
	if (size == 0)
		goto out;
	block = block_read(size, fd);
out:
	if (fd != -1)
		close(fd);
	return block;
}

void block_free(Block *blk) {
	if (!blk)
		return;
	free(blk->data);
	free(blk);
}

/* check whether block has enough free space to store len bytes */
bool block_capacity(Block *blk, size_t len) {
	return blk->size - blk->len >= len;
}

/* append data to block, assumes there is enough space available */
const char *block_append(Block *blk, const char *data, size_t len) {
	char *dest = memcpy(blk->data + blk->len, data, len);
	blk->len += len;
	return dest;
}

/* insert data into block at an arbitrary position, this should only be used with
 * data of the most recently created piece. */
bool block_insert(Block *blk, size_t pos, const char *data, size_t len) {
	if (pos > blk->len || !block_capacity(blk, len))
		return false;
	if (blk->len == pos)
		return block_append(blk, data, len);
	char *insert = blk->data + pos;
	memmove(insert + len, insert, blk->len - pos);
	memcpy(insert, data, len);
	blk->len += len;
	return true;
}

/* delete data from a block at an arbitrary position, this should only be used with
 * data of the most recently created piece. */
bool block_delete(Block *blk, size_t pos, size_t len) {
	size_t end;
	if (!addu(pos, len, &end) || end > blk->len)
		return false;
	if (blk->len == pos) {
		blk->len -= len;
		return true;
	}
	char *delete = blk->data + pos;
	memmove(delete, delete + len, blk->len - pos - len);
	blk->len -= len;
	return true;
}

Text *text_load(const char *filename) {
	return text_load_method(filename, TEXT_LOAD_AUTO);
}

static ssize_t write_all(int fd, const char *buf, size_t count) {
	size_t rem = count;
	while (rem > 0) {
		ssize_t written = write(fd, buf, rem > INT_MAX ? INT_MAX : rem);
		if (written < 0) {
			if (errno == EAGAIN || errno == EINTR)
				continue;
			return -1;
		} else if (written == 0) {
			break;
		}
		rem -= written;
		buf += written;
	}
	return count - rem;
}

static bool preserve_acl(int src, int dest) {
#if CONFIG_ACL
	acl_t acl = acl_get_fd(src);
	if (!acl)
		return errno == ENOTSUP ? true : false;
	if (acl_set_fd(dest, acl) == -1) {
		acl_free(acl);
		return false;
	}
	acl_free(acl);
#endif /* CONFIG_ACL */
	return true;
}

static bool preserve_selinux_context(int src, int dest) {
#if CONFIG_SELINUX
	char *context = NULL;
	if (!is_selinux_enabled())
		return true;
	if (fgetfilecon(src, &context) == -1)
		return errno == ENOTSUP ? true : false;
	if (fsetfilecon(dest, context) == -1) {
		freecon(context);
		return false;
	}
	freecon(context);
#endif /* CONFIG_SELINUX */
	return true;
}

static bool text_save_begin_inplace(TextSave *ctx) {
  wn_dbglog("ggg");
	struct stat now = { 0 };
	int saved_errno;
	if ((ctx->fd = open(ctx->filename, O_CREAT|O_WRONLY, 0666)) == -1)
		goto err;
	if (fstat(ctx->fd, &now) == -1)
		goto err;
	/* overwrite the existing file content, if something goes wrong
	 * here we are screwed, TODO: make a backup before? */
	if (ftruncate(ctx->fd, 0) == -1)
		goto err;
	ctx->type = TEXT_SAVE_INPLACE;
	return true;
err:
	saved_errno = errno;
	if (ctx->fd != -1)
		close(ctx->fd);
	ctx->fd = -1;
	errno = saved_errno;
	return false;
}

static bool text_save_commit_inplace(TextSave *ctx) {
  wn_dbglog("111");
	if (fsync(ctx->fd) == -1)
		return false;
	struct stat meta = { 0 };
  wn_dbglog("222");
	if (fstat(ctx->fd, &meta) == -1)
		return false;
  wn_dbglog("333");
	if (close(ctx->fd) == -1)
		return false;
  wn_dbglog("444");
	text_saved(ctx->txt, &meta);
	return true;
}

TextSave *text_save_begin(Text *txt, const char *filename, enum TextSaveMethod type) {
	if (!filename)
		return NULL;
	TextSave *ctx = calloc(1, sizeof *ctx);
	if (!ctx)
		return NULL;
	ctx->txt = txt;
	ctx->fd = -1;
	if (!(ctx->filename = strdup(filename)))
		goto err;
	errno = 0;
	if ((type == TEXT_SAVE_AUTO || type == TEXT_SAVE_INPLACE) && text_save_begin_inplace(ctx))
		return ctx;
err:
	text_save_cancel(ctx);
	return NULL;
}

bool text_save_commit(TextSave *ctx) {
	if (!ctx)
		return true;
	bool ret;
	switch (ctx->type) {
	case TEXT_SAVE_INPLACE:
		ret = text_save_commit_inplace(ctx);
		break;
	default:
		ret = false;
		break;
	}

	text_save_cancel(ctx);
	return ret;
}

void text_save_cancel(TextSave *ctx) {
	if (!ctx)
		return;
	int saved_errno = errno;
	if (ctx->fd != -1)
		close(ctx->fd);
	if (ctx->tmpname && ctx->tmpname[0])
		unlink(ctx->tmpname);
	free(ctx->tmpname);
	free(ctx->filename);
	free(ctx);
	errno = saved_errno;
}

bool text_save(Text *txt, const char *filename) {
	return text_save_method(txt, filename, TEXT_SAVE_AUTO);
}

bool text_save_method(Text *txt, const char *filename, enum TextSaveMethod method) {
	if (!filename) {
		text_saved(txt, NULL);
		return true;
	}
	TextSave *ctx = text_save_begin(txt, filename, method);
	if (!ctx)
		return false;
	Filerange range = (Filerange){ .start = 0, .end = text_size(txt) };
	ssize_t written = text_save_write_range(ctx, &range);
	if (written == -1 || (size_t)written != text_range_size(&range)) {
		text_save_cancel(ctx);
		return false;
	}
	return text_save_commit(ctx);
}

ssize_t text_save_write_range(TextSave *ctx, const Filerange *range) {
	return text_write_range(ctx->txt, range, ctx->fd);
}

ssize_t text_write(const Text *txt, int fd) {
	Filerange r = (Filerange){ .start = 0, .end = text_size(txt) };
	return text_write_range(txt, &r, fd);
}

ssize_t text_write_range(const Text *txt, const Filerange *range, int fd) {
	size_t size = text_range_size(range), rem = size;
	for (Iterator it = text_iterator_get(txt, range->start);
	     rem > 0 && text_iterator_valid(&it);
	     text_iterator_next(&it)) {
		size_t prem = it.end - it.text;
		if (prem > rem)
			prem = rem;
		ssize_t written = write_all(fd, it.text, prem);
		if (written == -1)
			return -1;
		rem -= written;
		if ((size_t)written != prem)
			break;
	}
	return size - rem;
}
