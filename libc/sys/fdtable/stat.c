/*
 * Copyright (C) 2020 Embecosm Limited
 * SPDX-License-Identifier: BSD-2-Clause
 */
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

extern int __stat_common (int file, struct stat *st);

/* Status of a file (by name).  */

int
_stat (const char *name, struct stat *st)
{
  int file;
  int res;
  memset (st, 0, sizeof (*st));

  file = _open (name, O_RDONLY);
  if (file == -1)
    return -1;

  res = __stat_common (file, st);

  _close (file);
  return res;
}
