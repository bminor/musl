#
# Makefile for musl (requires GNU make)
#
# This is how simple every makefile should be...
# No, I take that back - actually most should be less than half this size.
#
# Use config.mak to override any of the following variables.
# Do not make changes here.
#

exec_prefix = /usr/local
bindir = $(exec_prefix)/bin

prefix = /usr/local/musl
includedir = $(prefix)/include
libdir = $(prefix)/lib

SRCS = $(sort $(wildcard src/*/*.c))
OBJS = $(SRCS:.c=.o)
LOBJS = $(OBJS:.o=.lo)
GENH = include/bits/alltypes.h

CFLAGS  = -Os -nostdinc -ffreestanding -std=c99 -D_XOPEN_SOURCE=700 -pipe
LDFLAGS = -nostdlib -shared -fPIC -Wl,-e,_start -Wl,-Bsymbolic-functions
INC     = -I./include -I./src/internal -I./arch/$(ARCH)
PIC     = -fPIC -O3
AR      = $(CROSS_COMPILE)ar
RANLIB  = $(CROSS_COMPILE)ranlib
OBJCOPY = $(CROSS_COMPILE)objcopy

ALL_INCLUDES = $(sort $(wildcard include/*.h include/*/*.h) $(GENH))

EMPTY_LIB_NAMES = m rt pthread crypt util xnet resolv dl
EMPTY_LIBS = $(EMPTY_LIB_NAMES:%=lib/lib%.a)
CRT_LIBS = lib/crt1.o lib/crti.o lib/crtn.o
LIBC_LIBS = lib/libc.a
ALL_LIBS = $(LIBC_LIBS) $(CRT_LIBS) $(EMPTY_LIBS)

ALL_TOOLS = tools/musl-gcc

-include config.mak

all: $(ALL_LIBS) $(ALL_TOOLS)

install: $(ALL_LIBS:lib/%=$(DESTDIR)$(libdir)/%) $(ALL_INCLUDES:include/%=$(DESTDIR)$(includedir)/%) $(ALL_TOOLS:tools/%=$(DESTDIR)$(bindir)/%)

clean:
	rm -f crt/*.o
	rm -f $(OBJS)
	rm -f $(LOBJS)
	rm -f $(ALL_LIBS) lib/*.[ao] lib/*.so
	rm -f $(ALL_TOOLS)
	rm -f $(GENH) 
	rm -f include/bits

include/bits:
	@test "$(ARCH)" || { echo "Please set ARCH in config.mak before running make." ; exit 1 ; }
	ln -sf ../arch/$(ARCH)/bits $@

include/bits/alltypes.h.sh: include/bits

include/bits/alltypes.h: include/bits/alltypes.h.sh
	sh $< > $@

%.o: $(ARCH)/%.s
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

%.o: %.c $(GENH)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

%.lo: $(ARCH)/%.s
	$(CC) $(CFLAGS) $(INC) $(PIC) -c -o $@ $<

%.lo: %.c $(GENH)
	$(CC) $(CFLAGS) $(INC) $(PIC) -c -o $@ $<

lib/libc.so: $(LOBJS)
	$(CC) $(LDFLAGS) -o $@ $(LOBJS) -lgcc
	$(OBJCOPY) --weaken $@

lib/libc.a: $(OBJS)
	rm -f $@
	$(AR) rc $@ $(OBJS)
	$(RANLIB) $@

$(EMPTY_LIBS):
	rm -f $@
	$(AR) rc $@

lib/%.o: crt/%.o
	cp $< $@

tools/musl-gcc: tools/gen-musl-gcc.sh config.mak
	sh $< "$(prefix)" > $@ || { rm -f $@ ; exit 1 ; }
	chmod +x $@

$(DESTDIR)$(bindir)/%: tools/%
	install -D $< $@

$(DESTDIR)$(prefix)/%: %
	install -D -m 644 $< $@

.PRECIOUS: $(CRT_LIBS:lib/%=crt/%)

.PHONY: all clean install
