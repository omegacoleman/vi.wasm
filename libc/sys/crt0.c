#include "defs.h"

#include <fcntl.h>
#include <wn.h>

#include "fdtable/fdtable.h"

extern void exit(int code);
extern int main();

#define ASYNCIFY_STACKBUF_SIZE 4096

extern char** environ;

static char *__wn_environ[] = {
  "WASMNIX=0.1",
  "TERM=xterm-256color",
  NULL,
};

struct {
  i32 buf_begin;
  i32 buf_size;
  unsigned char buf[ASYNCIFY_STACKBUF_SIZE];
}__attribute__((packed)) __wn_asyncify_stackbuf;

extern wn_winch_handler_t __wn_winch_handler_p;

void __wn_asyncify_stackbuf_init() {
  __wn_asyncify_stackbuf.buf_begin = (i32) &__wn_asyncify_stackbuf.buf;
  __wn_asyncify_stackbuf.buf_size = (i32) (__wn_asyncify_stackbuf.buf_begin + ASYNCIFY_STACKBUF_SIZE);
}

WNEXPORT("asyncify_stackbuf_ptr") i32 __wn_asyncify_stackbuf_ptr() {
  return (i32) &__wn_asyncify_stackbuf;
}

WNEXPORT("entrance") void __wn_entrance() {
    environ = __wn_environ;
    __wn_asyncify_stackbuf_init();
    __init_fdtable();
    int ex = main();
    exit(ex);
}

