-include config.mk

include math/Makefile.inc
include argz/Makefile.inc
include stdlib/Makefile.inc
include ctype/Makefile.inc
include search/Makefile.inc
include stdio/Makefile.inc
include string/Makefile.inc
include reent/Makefile.inc
include time/Makefile.inc
include locale/Makefile.inc
include errno/Makefile.inc
include misc/Makefile.inc
include syscalls/Makefile.inc
include sys/Makefile.inc
include signal/Makefile.inc

libc_a_OBJS = $(libc_a_SOURCES:%.c=obj/%.o)

all: out/libc.a out/crt0.o

out/.tstamp:
	mkdir out
	touch out/.tstamp

obj/.tstamp:
	mkdir obj
	mkdir obj/argz
	mkdir obj/stdlib
	mkdir obj/ctype
	mkdir obj/search
	mkdir obj/stdio
	mkdir obj/string
	mkdir obj/reent
	mkdir obj/time
	mkdir obj/locale
	mkdir obj/errno
	mkdir obj/misc
	mkdir obj/syscalls
	mkdir obj/sys
	mkdir obj/signal
	mkdir obj/math
	touch obj/.tstamp

config.mk:
	@touch $@

$(libc_a_OBJS): obj/.tstamp config.mk
	${CC} ${CFLAGS} -o $@ -c $(@:obj/%.o=%.c)

-include obj/*/*.d

out/libc.a: ${libc_a_OBJS} out/.tstamp
	${AR} rcs $@ ${libc_a_OBJS} ${LDFLAGS}

out/crt0.o: sys/crt0.c config.mk
	${CC} ${CFLAGS} -o obj/sys/crt0.o -c sys/crt0.c
	cp obj/sys/crt0.o $@

clean:
	@echo cleaning
	@rm -rf obj out

.PHONY: all clean

