SRCDIR = .
CC = clang
CFLAGS = --target=wasm32 -isystem include -funsigned-char -std=gnu99 -D_LIBC -fno-builtin -nodefaultlibs -nostdlib -DNDEBUG -MMD -Wno-deprecated-non-prototype
CFLAGS_DEBUG = -U_FORTIFY_SOURCE -UNDEBUG -O0 -g
AR = llvm-ar

