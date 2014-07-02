#include <stdlib.h>
#include <string.h>
#include "locale_impl.h"
#include "libc.h"

locale_t __duplocale(locale_t old)
{
	locale_t new = calloc(1, sizeof *new + LOCALE_NAME_MAX + 1);
	if (!new) return 0;
	new->messages_name = (void *)(new+1);

	if (old == LC_GLOBAL_LOCALE) old = &libc.global_locale;
	new->ctype_utf8 = old->ctype_utf8;
	if (old->messages_name)
		strcpy(new->messages_name, old->messages_name);

	if (new && old != LC_GLOBAL_LOCALE) memcpy(new, old, sizeof *new);
	return new;
}

weak_alias(__duplocale, duplocale);
