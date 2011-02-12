#!/bin/sh

printf '#!/bin/sh\n\nlibc_prefix="%s"\n' "$1"

cat <<"EOF"
libc_lib=$libc_prefix/lib
libc_inc=$libc_prefix/include
libc_crt="$libc_lib/crt1.o"
libc_start="$libc_lib/crti.o"
libc_end="$libc_lib/crtn.o"

gcc_inc=$libc_inc
libgcc="`gcc \"$@\" -print-file-name=libgcc.a`"

gcc -wrapper sh,-c,'
x= ; y= ; z= ; s= ; for i ; do
  [ "$z" ] || set -- ; z=1
  case "$i" in
    -shared) s=1 ; set -- "$@" -shared ;;
    -Lxxxxxx) x=1 ;;
    -xxxxxx) x= ; [ "$s" ] || set -- "$@" "'"$libc_start"'" "'"$libc_crt"'" ;;
    -l*) [ "$y" ] || set -- "$@" '"$libc_end"' ; set -- "$@" "$i" ; y=1 ;;
    *) [ "$x" ] || set -- "$@" "$i" ;;
  esac
done
exec "$0" "$@"
' -std=gnu99 -nostdinc -nostdlib \
  -isystem "$libc_inc" -isystem "$gcc_inc" \
  -Wl,-xxxxxx "$@" -L"$libc_lib" -lc "$libgcc" -Lxxxxxx -Wl,-nostdlib
EOF
