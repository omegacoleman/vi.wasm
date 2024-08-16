/*
 * Copyright (C) 2020 Embecosm Limited
 * SPDX-License-Identifier: BSD-2-Clause
 */
#include "fdtable.h"

/* Close a file.  */
int
_close (int file)
{
  __remove_fdentry (file);
  return 0;
}
