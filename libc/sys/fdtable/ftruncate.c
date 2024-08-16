#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <wn.h>
#include "fdtable.h"

extern int errno;

int ftruncate(int file, off_t length) {
  struct fdentry *fd =__get_fdentry (file);
  if (fd == NULL)
    return -1;

  if (fd->ft_class != FTC_KV) {
    errno = EBADF;
    return -1;
  }

  if (length < 0) {
    errno = EINVAL;
    return -1;
  }

  return wn_kvsetlen(fd->kv_key, length);
}

