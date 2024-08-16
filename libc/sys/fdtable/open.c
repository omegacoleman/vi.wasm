/*
 * Copyright (C) 2020 Embecosm Limited
 * SPDX-License-Identifier: BSD-2-Clause
 */
#include "fdtable.h"
#include "wn.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>

extern int errno;

/* Open a file.  */
int
_open (const char *name, int flags, ...)
{
  /* HACK: ignore O_RDONLY O_WRONLY O_RDWR O_CREAT O_TRUNC */
  /* O_APPEND is unsupported and will err */
  if (flags & O_APPEND) {
    errno = EINVAL;
    return -1;
  }

  int file = __add_fdentry (FTC_KV);
  if (file == -1) return -1;
  struct fdentry* fd = __get_fdentry(file);
  if (fd == NULL) return -1;

  fd->kv_key = strdup(name);

  return file;
}
