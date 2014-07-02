#include <stdlib.h>
#include <string.h>
#include "locale_impl.h"
#include "libc.h"

locale_t __newlocale(int mask, const char *name, locale_t loc)
{
	int i;

	if (!loc) {
		loc = calloc(1, sizeof *loc + LOCALE_NAME_MAX + 1);
		if (!loc) return 0;
		loc->messages_name = (void *)(loc+1);
		for (i=0; i<LC_ALL; i++)
			if (!(mask & (1<<i)))
				__setlocalecat(loc, i, "");
	}

	for (i=0; i<LC_ALL; i++)
		if (mask & (1<<i))
			__setlocalecat(loc, i, name);

	return loc;
}

weak_alias(__newlocale, newlocale);
