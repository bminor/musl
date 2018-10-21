#include <stdlib.h>
#include <string.h>
#include "locale_impl.h"

int __loc_is_allocated(locale_t loc)
{
	return loc && loc != C_LOCALE && loc != UTF8_LOCALE;
}

locale_t __newlocale(int mask, const char *name, locale_t loc)
{
	struct __locale_struct tmp;

	for (int i=0; i<LC_ALL; i++) {
		tmp.cat[i] = (!(mask & (1<<i)) && loc) ? loc->cat[i] :
			__get_locale(i, (mask & (1<<i)) ? name : "");
		if (tmp.cat[i] == LOC_MAP_FAILED)
			return 0;
	}

	/* For locales with allocated storage, modify in-place. */
	if (__loc_is_allocated(loc)) {
		*loc = tmp;
		return loc;
	}

	/* Otherwise, first see if we can use one of the builtin locales.
	 * This makes the common usage case for newlocale, getting a C locale
	 * with predictable behavior, very fast, and more importantly, fail-safe. */
	if (!memcmp(&tmp, C_LOCALE, sizeof tmp)) return C_LOCALE;
	if (!memcmp(&tmp, UTF8_LOCALE, sizeof tmp)) return UTF8_LOCALE;

	/* If no builtin locale matched, attempt to allocate and copy. */
	if ((loc = malloc(sizeof *loc))) *loc = tmp;

	return loc;
}

weak_alias(__newlocale, newlocale);
