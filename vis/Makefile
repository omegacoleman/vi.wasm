-include config.mk

SRC = array.c \
	buffer.c \
	event-basic.c \
	libutf.c \
	main.c \
	map.c \
	sam.c \
	text-common.c \
	text-io.c \
	text-iterator.c \
	text-motions.c \
	text-objects.c \
	text-util.c \
	text.c \
	ui-terminal.c \
	view.c \
	vis-marks.c \
	vis-modes.c \
	vis-motions.c \
	vis-operators.c \
	vis-prompt.c \
	vis-registers.c \
	vis-text-objects.c \
	vis.c \
	driver-csi.c \
	termkey.c \
	osdep.c \
	tre/regcomp.c \
	tre/regexec.c \
	tre/tre-compile.c \
	tre/tre-match-approx.c \
	tre/tre-match-parallel.c \
	tre/tre-parse.c \
	tre/xmalloc.c \
	tre/regerror.c \
	tre/tre-ast.c \
	tre/tre-filter.c \
	tre/tre-match-backtrack.c \
	tre/tre-mem.c \
	tre/tre-stack.c \
	text-regex-tre.c
OBJ = $(SRC:%.c=obj/%.o)

VERSION = v0.9-wasm

CONFIG_HELP ?= 1
CONFIG_CURSES ?= 0

CFLAGS += -DVERSION=\"${VERSION}\"
CFLAGS += -DCONFIG_HELP=${CONFIG_HELP}
CFLAGS += -DCONFIG_CURSES=${CONFIG_CURSES}

all: vis.wasm

config.h:
	cp config.def.h config.h

config.mk:
	@touch $@

obj/.tstamp:
	mkdir obj
	mkdir obj/tre
	touch obj/.tstamp

obj/main.o: config.h

$(OBJ): config.mk obj/.tstamp
	${CC} ${CFLAGS} ${CFLAGS_EXTRA} -o $@ -c $(@:obj/%.o=%.c)

-include obj/*.d
-include obj/tre/*.d

.vis-sync.wasm: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS} ${LDFLAGS_EXTRA}

vis.wasm: .vis-sync.wasm
	${ASYNCIFY} -o $@ .vis-sync.wasm

debug: clean
	@$(MAKE) CFLAGS_EXTRA='${CFLAGS_EXTRA} ${CFLAGS_DEBUG}'

clean:
	@echo cleaning
	@rm -rf obj
	@rm -f vis.wasm .vis-sync.wasm

