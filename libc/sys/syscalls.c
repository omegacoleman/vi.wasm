#include "defs.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/times.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define WN_DEVICE_META 1
#define WN_DEVICE_TTY 2
#define WN_DEVICE_BLK 3

#define WN_OP_READ 1
#define WN_OP_WRITE 2

WNGLUE("ttyread") i32 __wn_ttyread(i32 buffer, i32 end);
WNGLUE("ttywrite") i32 __wn_ttywrite(i32 buffer, i32 end);
WNGLUE("ttywait") i32 __wn_ttywait(i32 timeout_ms);
WNGLUE("ttysize") void __wn_ttysize(i32 waddr, i32 haddr);
WNGLUE("growmem") i32 __wn_growmem(i32 incr);
WNGLUE("exit") void __wn_exit(i32 status);
WNGLUE("dbglog") i32 __wn_dbglog(i32 buffer, i32 end);

static int nosys() {
  errno = ENOSYS;
  return -1;
}

int wn_ttyread(char* ptr, int len) {
  return (int) (__wn_ttyread((i32) ptr, (i32) (ptr + len)) - (i32)ptr);
}

int wn_ttywrite(char* ptr, int len) {
  return (int) (__wn_ttywrite((i32) ptr, (i32) (ptr + len)) - (i32)ptr);
}

int wn_ttywait(int timeout_ms) {
  return (int) __wn_ttywait((i32) timeout_ms);
}

void wn_dbglog(char* log) {
  __wn_dbglog((i32) log, (i32) (log + strlen(log)));
}

void wn_ttysize(int *w, int *h) {
  i32 ww __attribute__((aligned(4)));;
  i32 hh __attribute__((aligned(4)));
  __wn_ttysize((i32) &w, (i32) &h);
  *w = ww;
  *h = hh;
}

void _exit(int status) {
  __wn_exit(status);
}

int _close(int file) {
  return nosys();
}

int _execve(char *name, char **argv, char **env) {
  return nosys();
}

int _fork() {
  return nosys();
}

int _fstat(int file, struct stat *st) {
  return nosys();
}

int _getpid() {
  return 1;
}

int _isatty(int file) {
  switch (file) {
    case STDIN_FILENO: // fallthrough
    case STDOUT_FILENO: // fallthrough
    case STDERR_FILENO: return 1;
    default: return 0;
  }
}

int _kill(int pid, int sig) {
  return nosys();
}

int _link(char *old, char *new) {
  return nosys();
}

int _lseek(int file, int ptr, int dir) {
  return nosys();
}

int _open(const char *name, int flags, ...) {
  return nosys();
}

int _read(int file, char *ptr, int len) {
  switch (file) {
    case STDIN_FILENO: // fallthrough
    case STDOUT_FILENO: // fallthrough
    case STDERR_FILENO: return wn_ttyread(ptr, len);
    default: return nosys();
  }
}

caddr_t _sbrk(int incr) {
  return (caddr_t) __wn_growmem((i32) incr);
}

int _stat(const char *file, struct stat *st) {
  return nosys();
}

clock_t _times(struct tms *buf) {
  return (clock_t) nosys();
}

int _unlink(char *name) {
  return nosys();
}

int _wait(int *status) {
  return nosys();
}

int _write(int file, char *ptr, int len) {
  switch (file) {
    case STDIN_FILENO: // fallthrough
    case STDOUT_FILENO: // fallthrough
    case STDERR_FILENO: return wn_ttywrite(ptr, len);
    default: return nosys();
  }
}

int _gettimeofday(struct timeval *p, void *z) {
  return nosys();
}

int fsync(int fd) {
  return 0;
}

int ftruncate(int fd, off_t length) {
  return nosys();
}

int _fcntl(int fd, int cmd, ...) {
  return nosys();
}

int _mkdir(const char * pathname, mode_t mode) {
  return nosys();
}

int _rename(const char * oldpath, const char *newpath) {
  return nosys();
}

int _getentropy(void *buffer, size_t length) {
  return nosys();
}

