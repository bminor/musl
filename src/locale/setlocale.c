#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include "locale_impl.h"
#include "libc.h"
#include "atomic.h"

static char buf[2+4*(LOCALE_NAME_MAX+1)];

char *setlocale(int cat, const char *name)
{
	struct __locale_map *lm;
	int i, j;

	if (!libc.global_locale.messages_name) {
		libc.global_locale.messages_name =
			buf + 2 + 3*(LOCALE_NAME_MAX+1);
	}

	if ((unsigned)cat > LC_ALL) return 0;

	/* For LC_ALL, setlocale is required to return a string which
	 * encodes the current setting for all categories. The format of
	 * this string is unspecified, and only the following code, which
	 * performs both the serialization and deserialization, depends
	 * on the format, so it can easily be changed if needed. */
	if (cat == LC_ALL) {
		if (name) {
			char part[LOCALE_NAME_MAX+1];
			if (name[0] && name[1]==';'
			    && strlen(name) > 2 + 3*(LOCALE_NAME_MAX+1)) {
				part[0] = name[0];
				part[1] = 0;
				setlocale(LC_CTYPE, part);
				part[LOCALE_NAME_MAX] = 0;
				for (i=LC_TIME; i<LC_MESSAGES; i++) {
					memcpy(part, name + 2 + (i-2)*(LOCALE_NAME_MAX+1), LOCALE_NAME_MAX);
					for (j=LOCALE_NAME_MAX-1; j && part[j]==';'; j--)
						part[j] = 0;
					setlocale(i, part);
				}
				setlocale(LC_MESSAGES, name + 2 + 3*(LOCALE_NAME_MAX+1));
			} else {
				for (i=0; i<LC_ALL; i++)
					setlocale(i, name);
			}
		}
		memset(buf, ';', 2 + 3*(LOCALE_NAME_MAX+1));
		buf[0] = libc.global_locale.ctype_utf8 ? 'U' : 'C';
		for (i=LC_TIME; i<LC_MESSAGES; i++) {
			lm = libc.global_locale.cat[i-2];
			if (lm) memcpy(buf + 2 + (i-2)*(LOCALE_NAME_MAX+1),
				lm->name, strlen(lm->name));
		}
		return buf;
	}

	if (name) {
		int adj = libc.global_locale.ctype_utf8;
		__setlocalecat(&libc.global_locale, cat, name);
		adj -= libc.global_locale.ctype_utf8;
		if (adj) a_fetch_add(&libc.bytelocale_cnt_minus_1, adj);
	}

	switch (cat) {
	case LC_CTYPE:
		return libc.global_locale.ctype_utf8 ? "C.UTF-8" : "C";
	case LC_NUMERIC:
		return "C";
	case LC_MESSAGES:
		return libc.global_locale.messages_name[0]
			? libc.global_locale.messages_name : "C";
	default:
		lm = libc.global_locale.cat[cat-2];
		return lm ? lm->name : "C";
	}
}
