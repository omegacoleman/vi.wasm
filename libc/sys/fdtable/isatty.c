/*
 * Copyright (C) 2020 Embecosm Limited
 * SPDX-License-Identifier: BSD-2-Clause
 */
#include <sys/stat.h>
#include "fdtable.h"

int
_isatty (int file)
{
  struct fdentry *fd =__get_fdentry (file);
  if (fd == NULL)
    return -1;
  if (fd->ft_class == FTC_TTY) return 1;
  return 0;
}
