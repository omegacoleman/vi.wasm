/*
 * Copyright (C) 2020 Embecosm Limited
 * SPDX-License-Identifier: BSD-2-Clause
 */
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "fdtable.h"
#include "wn.h"

extern int errno;

/* Set position in a file.  */
off_t
_lseek (int file, off_t offset, int dir)
{
  struct fdentry *fd;
  off_t abs_pos;
  off_t flen;

  fd =__get_fdentry (file);
  if (fd == NULL || fd->ft_class != FTC_KV)
  {
    errno = EBADF;
    return -1;
  }

  if (dir == SEEK_CUR && offset == 0)
    return fd->pos;

  switch (dir)
  {
    case SEEK_SET:
      abs_pos = offset;
      break;
    case SEEK_CUR:
      abs_pos = fd->pos + offset;
      break;
    case SEEK_END:
      flen = wn_kvlen(fd->kv_key);
      if (flen == -1)
        return -1;
      abs_pos = flen + offset;
      break;
    default:
      errno = EINVAL;
      return -1;
  }

  if (abs_pos < 0)
  {
    errno = EINVAL;
    return -1;
  }

  fd->pos = abs_pos;
  return abs_pos;
}
