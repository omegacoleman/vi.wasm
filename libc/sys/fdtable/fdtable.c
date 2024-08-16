/*
 * Copyright (C) 2020 Embecosm Limited
 * SPDX-License-Identifier: BSD-2-Clause
 */
#include "fdtable.h"
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef WN_MAX_OPEN_FILES
#define WN_MAX_OPEN_FILES 16
#endif

static struct fdentry fdtable[WN_MAX_OPEN_FILES];

void
__init_fdtable ()
{
  fdtable[STDIN_FILENO].ft_class = FTC_TTY;
  fdtable[STDOUT_FILENO].ft_class = FTC_TTY;
  fdtable[STDERR_FILENO].ft_class = FTC_TTY;
}

int
__add_fdentry (int ft_class)
{
  for (int i=0; i<WN_MAX_OPEN_FILES; i++)
    if (fdtable[i].ft_class == FTC_AVAILABLE)
    {
      fdtable[i].ft_class = ft_class;
      fdtable[i].pos = 0;
      return i;
    }
  /* Too many open files.  */
  errno = ENFILE;
  return -1;
}

/* Return the fdentry for file or NULL if not found.  */
struct fdentry *
__get_fdentry (int file)
{
  if (file<0 || file>=WN_MAX_OPEN_FILES || fdtable[file].ft_class == FTC_AVAILABLE)
  {
    errno = EBADF;
    return NULL;
  }
  return &fdtable[file];
}

void
__remove_fdentry (int file)
{
  if (fdtable[file].kv_key != NULL) {
    free(fdtable[file].kv_key);
    fdtable[file].kv_key = NULL;
  }
  fdtable[file].ft_class = FTC_AVAILABLE;
}

