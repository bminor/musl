#include <stdlib.h>
#include <string.h>
#include "locale_impl.h"
#include "libc.h"

locale_t __newlocale(int mask, const char *name, locale_t loc)
{
	int i;

	if (!loc) {
		loc = malloc(sizeof *loc);
		if (!loc) return 0;
		for (i=0; i<LC_ALL; i++)
			if (!(mask & (1<<i)))
				loc->cat[i] = __get_locale(i, "");
	}

	for (i=0; i<LC_ALL; i++)
		if (mask & (1<<i))
			loc->cat[i] = __get_locale(i, name);

	return loc;
}

weak_alias(__newlocale, newlocale);
