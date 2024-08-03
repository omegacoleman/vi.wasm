#ifndef OSDEP_H
#define OSDEP_H

void osdep_init();

void ttysize(int* width, int* height);

int waitfd(int fd, int timeout, int ret_eintr);

int need_resize();
void clear_resize();

#endif

