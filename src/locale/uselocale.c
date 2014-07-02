#include "locale_impl.h"
#include "pthread_impl.h"
#include "libc.h"

locale_t __uselocale(locale_t new)
{
	pthread_t self = __pthread_self();
	locale_t old = self->locale;
	locale_t global = &libc.global_locale;

	if (new == LC_GLOBAL_LOCALE) new = global;

	if (new && new != old) {
		int adj = 0;
		if (new == global) a_dec(&libc.uselocale_cnt);
		else if (!new->ctype_utf8) adj++;
		if (old == global) a_inc(&libc.uselocale_cnt);
		else if (!old->ctype_utf8) adj--;
		a_fetch_add(&libc.bytelocale_cnt_minus_1, adj);
		self->locale = new;
	}

	return old == global ? LC_GLOBAL_LOCALE : old;
}

weak_alias(__uselocale, uselocale);
