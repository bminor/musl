#include <locale.h>
#include <stdlib.h>
#include "libc.h"
#include "pthread_impl.h"

#define LOCALE_NAME_MAX 15

int __setlocalecat(locale_t, int, const char *);

#define CURRENT_LOCALE \
	(libc.uselocale_cnt ? __pthread_self()->locale : &libc.global_locale)

#define CURRENT_UTF8 \
	(libc.bytelocale_cnt_minus_1<0 || __pthread_self()->locale->ctype_utf8)

#undef MB_CUR_MAX
#define MB_CUR_MAX (CURRENT_UTF8 ? 4 : 1)
