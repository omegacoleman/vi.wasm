/*
 * Copyright (C) 2020 Embecosm Limited
 * SPDX-License-Identifier: BSD-2-Clause
 */
#include <sys/stat.h>
#include <wn.h>
#include "fdtable.h"

/* Used by _fstat and _stat to fill in some common details.  */

int
__stat_common (int file, struct stat *st)
{
  struct fdentry *fd =__get_fdentry (file);

  st->st_mode |= S_IREAD | S_IWRITE;

  if (fd->ft_class == FTC_KV) {
    st->st_mode |= S_IFREG;
    st->st_blksize = 4096;
  }

  if (fd->ft_class == FTC_TTY) {
    st->st_mode |= S_IFCHR | S_IFIFO;
  }

  /* Attempt to get length of file.  */
  off_t flen = wn_kvlen(fd->kv_key);
  if (flen == -1)
    return -1;

  st->st_size = flen;

  return 0;
}
