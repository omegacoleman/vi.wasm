#include "defs.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/times.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <wn.h>

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define WN_DEVICE_META 1
#define WN_DEVICE_TTY 2
#define WN_DEVICE_BLK 3

#define WN_OP_READ 1
#define WN_OP_WRITE 2

#define EWINCH 4

WNGLUE("ttyread") i32 __wn_ttyread(i32 buffer, i32 end);
WNGLUE("ttywrite") i32 __wn_ttywrite(i32 buffer, i32 end);
WNGLUE("ttywait") i32 __wn_ttywait(i32 timeout_ms);
WNGLUE("ttysize") void __wn_ttysize(i32 waddr, i32 haddr);

WNGLUE("kvpread") i32 __wn_kvpread(i32 key, i32 keyend, i32 pos, i32 buffer, i32 end);
WNGLUE("kvpwrite") i32 __wn_kvpwrite(i32 key, i32 keyend, i32 pos, i32 buffer, i32 end);
WNGLUE("kvlen") i32 __wn_kvlen(i32 key, i32 keyend);
WNGLUE("kvsetlen") i32 __wn_kvsetlen(i32 key, i32 keyend, i32 len);

WNGLUE("growmem") i32 __wn_growmem(i32 incr);
WNGLUE("exit") void __wn_exit(i32 status);
WNGLUE("dbglog") i32 __wn_dbglog(i32 buffer, i32 end);

wn_winch_handler_t __wn_winch_handler_p = NULL;

static int __wn_nosys(const char* msg) {
  wn_dbglog(msg);
  errno = ENOSYS;
  return -1;
}

#define nosys(__fn) __wn_nosys("nosys: "__fn)

int wn_ttyread(char* ptr, int len) {
  int ret = (int) __wn_ttyread((i32) ptr, (i32) (ptr + len));
  if (ret == -EWINCH) {
    if (__wn_winch_handler_p) __wn_winch_handler_p();
    errno = EINTR;
    return -1;
  }
  if (ret < 0) {
    errno = -ret;
    return -1;
  }
  return ret - ((i32) ptr);
}

int wn_ttywrite(const char* ptr, int len) {
  int ret = (int) __wn_ttywrite((i32) ptr, (i32) (ptr + len));
  if (ret < 0) {
    errno = -ret;
    return -1;
  }
  return ret - ((i32) ptr);
}

int wn_ttywait(int timeout_ms) {
  int ret = (int) __wn_ttywait((i32) timeout_ms);
  if (ret == -EWINCH) {
    if (__wn_winch_handler_p) __wn_winch_handler_p();
    errno = EINTR;
    return -1;
  }
  if (ret < 0) {
    errno = -ret;
    return -1;
  }
  return ret;
}

void wn_dbglog(const char* log) {
  __wn_dbglog((i32) log, (i32) (log + strlen(log)));
}

void wn_ttysize(int *w, int *h) {
  i32 ww __attribute__((aligned(4)));
  i32 hh __attribute__((aligned(4)));
  __wn_ttysize((i32) &ww, (i32) &hh);
  *w = ww;
  *h = hh;
}

int wn_kvpread(const char* key, off_t off, char* ptr, int len) {
  int ret = (int) __wn_kvpread((i32) key, (i32) (key + strlen(key)), (i32) off, (i32) ptr, (i32) (ptr + len));
  if (ret < 0) {
    errno = -ret;
    return -1;
  }
  return ret - ((i32) ptr);
}

int wn_kvpwrite(const char* key, off_t off, const char* ptr, int len) {
  int ret = (int) __wn_kvpwrite((i32) key, (i32) (key + strlen(key)), (i32) off, (i32) ptr, (i32) (ptr + len));
  if (ret < 0) {
    errno = -ret;
    return -1;
  }
  return ret - ((i32) ptr);
}

off_t wn_kvlen(const char* key) {
  int ret = (int) __wn_kvlen((i32) key, (i32) (key + strlen(key)));
  if (ret < 0) {
    errno = -ret;
    return -1;
  }
  return ret;
}

int wn_kvsetlen(const char* key, off_t len) {
  int ret = (int) __wn_kvsetlen((i32) key, (i32) (key + strlen(key)), (i32) len);
  if (ret < 0) {
    errno = -ret;
    return -1;
  }
  return 0;
}

void _exit(int status) {
  __wn_exit(status);
}

int _execve(char *name, char **argv, char **env) {
  return nosys("execve");
}

int _fork() {
  return nosys("fork");
}

int _getpid() {
  return 1;
}

int _kill(int pid, int sig) {
  return nosys("kill");
}

int _link(char *old, char *new) {
  return nosys("link");
}

caddr_t _sbrk(int incr) {
  return (caddr_t) __wn_growmem((i32) incr);
}

clock_t _times(struct tms *buf) {
  return (clock_t) nosys();
}

int _unlink(char *name) {
  return nosys("unlink");
}

int _wait(int *status) {
  return nosys("wait");
}

int _gettimeofday(struct timeval *p, void *z) {
  return nosys("gettimeofday");
}

int fsync(int fd) {
  return 0;
}

int _fcntl(int fd, int cmd, ...) {
  return nosys("fcntl");
}

int _mkdir(const char * pathname, mode_t mode) {
  return nosys("mkdir");
}

int _rename(const char * oldpath, const char *newpath) {
  return nosys("rename");
}

int _getentropy(void *buffer, size_t length) {
  return nosys("getentrophy");
}

void wn_set_winch_handler(wn_winch_handler_t func) {
  __wn_winch_handler_p = func;
}

