#include <locale.h>
#include <string.h>
#include "locale_impl.h"
#include "libc.h"
#include "atomic.h"

static const char envvars[][12] = {
	"LC_CTYPE",
	"LC_NUMERIC",
	"LC_TIME",
	"LC_COLLATE",
	"LC_MONETARY",
	"LC_MESSAGES",
};

int __setlocalecat(locale_t loc, int cat, const char *val)
{
	if (!*val) {
		(val = getenv("LC_ALL")) ||
		(val = getenv(envvars[cat])) ||
		(val = getenv("LANG")) ||
		(val = "C.UTF-8");
	}

	size_t n = strnlen(val, LOCALE_NAME_MAX);
	int builtin = (val[0]=='C' && !val[1])
		|| !strcmp(val, "C.UTF-8")
		|| !strcmp(val, "POSIX");

	switch (cat) {
	case LC_CTYPE:
		a_store(&loc->ctype_utf8, !builtin || val[1]=='.');
		break;
	case LC_MESSAGES:
		if (builtin) {
			loc->messages_name[0] = 0;
		} else {
			memcpy(loc->messages_name, val, n);
			loc->messages_name[n] = 0;
		}
		/* fall through */
	default:
		break;
	}
	return 0;
}
