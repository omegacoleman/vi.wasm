#ifndef OSDEP_H
#define OSDEP_H

void ttysize(int* width, int* height);

int waitfd(int fd, int timeout, int ret_eintr);

#endif

