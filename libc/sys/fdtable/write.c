/*
 * Copyright (C) 2020 Embecosm Limited
 * SPDX-License-Identifier: BSD-2-Clause
 */
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include "fdtable.h"
#include "wn.h"

extern int errno;

/* Write to a file.  */
ssize_t
_write (int file, const void *ptr, size_t len)
{
  struct fdentry *fd =__get_fdentry (file);
  if (fd == NULL)
    return -1;

  switch(fd->ft_class) {
    case FTC_TTY:
    return wn_ttywrite((char*) ptr, len);
    case FTC_KV:
    return wn_kvpwrite(fd->kv_key, fd->pos, (char*) ptr, len);
    default:
    errno = EBADF;
    return -1;
  }
}
