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
syslibdir = /lib

SRCS = $(sort $(wildcard src/*/*.c))
OBJS = $(SRCS:.c=.o)
LOBJS = $(OBJS:.o=.lo)
GENH = include/bits/alltypes.h

CFLAGS  = -Os -nostdinc -ffreestanding -std=c99 -D_XOPEN_SOURCE=700 -pipe
LDFLAGS = -nostdlib -shared -fPIC -Wl,-e,_start -Wl,-Bsymbolic-functions
INC     = -I./src/internal -I./include -I./arch/$(ARCH)
PIC     = -fPIC -O3
AR      = $(CROSS_COMPILE)ar
RANLIB  = $(CROSS_COMPILE)ranlib
OBJCOPY = $(CROSS_COMPILE)objcopy

ALL_INCLUDES = $(sort $(wildcard include/*.h include/*/*.h) $(GENH))

EMPTY_LIB_NAMES = m rt pthread crypt util xnet resolv dl
EMPTY_LIBS = $(EMPTY_LIB_NAMES:%=lib/lib%.a)
CRT_LIBS = lib/crt1.o lib/crti.o lib/crtn.o
STATIC_LIBS = lib/libc.a $(EMPTY_LIBS)
SHARED_LIBS = lib/libc.so
TOOL_LIBS = lib/musl-gcc.specs
ALL_LIBS = $(CRT_LIBS) $(STATIC_LIBS) $(SHARED_LIBS) $(TOOL_LIBS)
ALL_TOOLS = tools/musl-gcc

LDSO_PATHNAME = $(syslibdir)/ld-musl-$(ARCH).so.1

-include config.mak

all: $(ALL_LIBS) $(ALL_TOOLS)

install: $(ALL_LIBS:lib/%=$(DESTDIR)$(libdir)/%) $(ALL_INCLUDES:include/%=$(DESTDIR)$(includedir)/%) $(ALL_TOOLS:tools/%=$(DESTDIR)$(bindir)/%) $(if $(SHARED_LIBS),$(DESTDIR)$(LDSO_PATHNAME),)

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
	$(CC) $(LDFLAGS) -Wl,-soname=libc.so -o $@ $(LOBJS) -lgcc
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

lib/musl-gcc.specs: tools/musl-gcc.specs.sh config.mak
	sh $< "$(includedir)" "$(libdir)" "$(LDSO_PATHNAME)" > $@

tools/musl-gcc: config.mak
	printf '#!/bin/sh\nexec gcc "$$@" -specs "%s/musl-gcc.specs"\n' "$(libdir)" > $@
	chmod +x $@

$(DESTDIR)$(bindir)/%: tools/%
	install -D $< $@

$(DESTDIR)$(libdir)/%.so: lib/%.so
	install -D -m 755 $< $@

$(DESTDIR)$(libdir)/%: lib/%
	install -D -m 644 $< $@

$(DESTDIR)$(includedir)/%: include/%
	install -D -m 644 $< $@

$(DESTDIR)$(LDSO_PATHNAME): lib/libc.so
	install -d -m 755 $(DESTDIR)$(syslibdir) || true
	ln -sf $(libdir)/libc.so $@ || true

.PRECIOUS: $(CRT_LIBS:lib/%=crt/%)

.PHONY: all clean install
