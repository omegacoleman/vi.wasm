SRCDIR = .
CC = clang

LIBCDIR = ../libc
CFLAGS = --target=wasm32 -isystem $(LIBCDIR)/include -funsigned-char -std=gnu99 -fno-builtin -nodefaultlibs -nostdlib -DNDEBUG -MMD -Wno-deprecated-non-prototype -DHAVE_MEMRCHR=1 -U_XOPEN_SOURCE -D_XOPEN_SOURCE=700 -DMBEDVIS
CFLAGS_DEBUG = -U_FORTIFY_SOURCE -UNDEBUG -O0 -g3 -ggdb
COMPILER_RUNTIME = ../lib/libclang_rt.builtins-wasm32.a
LDFLAGS = --target=wasm32 --no-standard-libraries -Wl,--no-entry $(LIBCDIR)/out/crt0.o -L$(LIBCDIR)/out -lc $(COMPILER_RUNTIME)

CONFIG_HELP = 1
CONFIG_CURSES = 0

CFLAGS_CURSES =
LDFLAGS_CURSES =

ASYNCIFY = wasm-opt --asyncify

