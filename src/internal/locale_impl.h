#include <locale.h>
#include <stdlib.h>
#include "libc.h"
#include "pthread_impl.h"

#define LOCALE_NAME_MAX 15

struct __locale_map {
	const void *map;
	size_t map_size;
	char name[LOCALE_NAME_MAX+1];
	struct __locale_map *next;
};

int __setlocalecat(locale_t, int, const char *);
const char *__mo_lookup(const void *, size_t, const char *);
const char *__lctrans(const char *, const struct __locale_map *);
const char *__lctrans_cur(const char *);

#define LCTRANS(msg, lc, loc) __lctrans(msg, (loc)->cat[(lc)-2])
#define LCTRANS_CUR(msg) __lctrans_cur(msg)

#define CURRENT_LOCALE \
	(libc.uselocale_cnt ? __pthread_self()->locale : &libc.global_locale)

#define CURRENT_UTF8 \
	(libc.bytelocale_cnt_minus_1<0 || __pthread_self()->locale->ctype_utf8)

#undef MB_CUR_MAX
#define MB_CUR_MAX (CURRENT_UTF8 ? 4 : 1)
