#include "osdep.h"

#ifndef MBEDVIS

#include <sys/ioctl.h>
#include <sys/poll.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

volatile sig_atomic_t a_need_resize = 0;

void sigwinch_handler(int dig) {
  a_need_resize = 1;
}

void osdep_init() {
  signal(SIGWINCH, sigwinch_handler);
}

int need_resize() {
  return a_need_resize;
}

void clear_resize() {
  a_need_resize = 0;
}

void ttysize(int* width, int* height) {
  struct winsize ws;
	if (ioctl(STDERR_FILENO, TIOCGWINSZ, &ws) != -1) {
		if (ws.ws_col > 0)
			*width = ws.ws_col;
		if (ws.ws_row > 0)
			*height = ws.ws_row;
	}
}

int waitfd(int rfd, int timeout, int ret_eintr) {
  struct pollfd fd;

retry:
  fd.fd = rfd;
  fd.events = POLLIN;

  int pollret = poll(&fd, 1, timeout);
  if(pollret == -1) {
    if(errno == EINTR && !ret_eintr)
      goto retry;
  }

  return pollret;
}

#else

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <wn.h>

int flag_need_resize = 0;

void winch_handler() {
  flag_need_resize = 1;
}

void osdep_init() {
  wn_set_winch_handler(&winch_handler);
}

int need_resize() {
  return flag_need_resize;
}

void clear_resize() {
  flag_need_resize = 0;
}

void ttysize(int* width, int* height) {
  wn_ttysize(width, height);
}

int waitfd(int rfd, int timeout, int ret_eintr) {
  switch (rfd) {
    case STDIN_FILENO: // fallthrough
    case STDOUT_FILENO: // fallthrough
    case STDERR_FILENO:
    return wn_ttywait(timeout);
    default:
    return 1;
  }
}

char *dirname(char* s) {
	size_t i;
	if (!s || !*s) return ".";
	i = strlen(s)-1;
	for (; s[i]=='/'; i--) if (!i) return "/";
	for (; s[i]!='/'; i--) if (!i) return ".";
	for (; s[i]=='/'; i--) if (!i) return "/";
	s[i+1] = 0;
	return s;
}

char *basename(char* s) {
	size_t i;
	if (!s || !*s) return ".";
	i = strlen(s)-1;
	for (; i&&s[i]=='/'; i--) s[i] = 0;
	for (; i&&s[i-1]!='/'; i--);
	return s+i;
}

char* realpath(const char *path, char* resolved) {
  strcpy(resolved, path);
  return resolved;
}

int chdir(const char* path) {
  errno = ENOENT;
  return -1;
}

char* getcwd(char* buf, size_t size) {
  if (size < 2) {
    errno = ERANGE;
    return NULL;
  }
  buf[0] = '/';
  buf[1] = '\0';
  return buf;
}

#endif

