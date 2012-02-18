#!/bin/sh

printf '#!/bin/sh\n\nlibc_prefix="%s"\nldso_pathname="%s"\n' "$1" "$2"

cat <<"EOF"
gcc=gcc
libc_lib=$libc_prefix/lib
libc_inc=$libc_prefix/include
libc_crt="$libc_lib/crt1.o"
libc_start="$libc_lib/crti.o"
libc_end="$libc_lib/crtn.o"

gcc_inc=$libc_inc
libgcc="$("$gcc" -print-file-name=libgcc.a)"
libgcc=${libgcc%/libgcc.a}

gccver=${libgcc##*/}
gcctarget=${libgcc%/*}
gcctarget=${gcctarget##*/}

case "$gccver" in
[0123].*|4.[01]*) ;;
*) nosp=-fno-stack-protector ;;
esac

[ "x$1" = "x-V" ] && { printf "%s: -V not supported\n" "$0" ; exit 1 ; }

for i ; do
case "$skip$i" in
-I|-L) skip=--- ; continue ;;
-[cSE]|-M*) nolink=1 ;;
-shared|-nostartfiles|-nostdlib) nocrt=1 ;;
-*) ;;
*) havefile=1 ;;
esac
skip=
done

[ "$havefile" ] || nolink=1

[ "$nolink" ] && nocrt=1

[ "$nocrt" ] || set -- "$libc_start" "$libc_crt" "$@" "$libc_end" \

[ "$nolink" ] || {
tmp_specs=$HOME/.specs.tmp.$$
printf '*link_libgcc:\n\n\n' > "$tmp_specs" || exit 1
exec 3<"$tmp_specs"
rm -f "$tmp_specs"
set -- -specs=/proc/self/fd/3 "$@" \
  -Wl,--as-needed -Wl,--start-group -lc -lgcc -lgcc_eh -Wl,--end-group \
  -Wl,-dynamic-linker,"$ldso_pathname" -Wl,-nostdlib
}

set -- -nostdinc -nostdlib $nosp \
  -isystem "$libc_inc" -isystem "$gcc_inc" "$@" \
  -L"$libc_lib" -L"$libgcc"

exec "$gcc" "$@"
EOF
