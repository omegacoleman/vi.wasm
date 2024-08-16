/*
 * Copyright (C) 2020 Embecosm Limited
 * SPDX-License-Identifier: BSD-2-Clause
 */
#include <sys/types.h>

#ifndef WN_FDTABLE_H
#define WN_FDTABLE_H

#define FTC_AVAILABLE 0
#define FTC_TTY 4
#define FTC_KV 5

extern void __init_fdtable ();
extern int __add_fdentry (int);
extern struct fdentry * __get_fdentry (int);
extern void __remove_fdentry (int);

struct fdentry
{
  int ft_class;
  char* kv_key;
  off_t pos;
};

#endif
