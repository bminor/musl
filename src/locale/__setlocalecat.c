#include <locale.h>
#include <string.h>
#include "locale_impl.h"
#include "libc.h"
#include "atomic.h"

const char *__lctrans_impl(const char *msg, const struct __locale_map *lm)
{
	const char *trans = 0;
	if (lm) trans = __mo_lookup(lm->map, lm->map_size, msg);
	return trans ? trans : msg;
}

const unsigned char *__map_file(const char *, size_t *);
int __munmap(void *, size_t);
char *__strchrnul(const char *, int);

static struct __locale_map *findlocale(const char *name, size_t n)
{
	static void *volatile loc_head;
	struct __locale_map *p, *new, *old_head;
	const char *path = 0, *z;
	char buf[256];
	size_t l;
	const void *map;
	size_t map_size;

	for (p=loc_head; p; p=p->next)
		if (!strcmp(name, p->name)) return p;

	if (!libc.secure) path = getenv("MUSL_LOCPATH");
	/* FIXME: add a default path? */
	if (!path) return 0;

	for (; *path; path=z+!!*z) {
		z = __strchrnul(path, ':');
		l = z - path - !!*z;
		if (l >= sizeof buf - n - 2) continue;
		memcpy(buf, path, l);
		buf[l] = '/';
		memcpy(buf+l+1, name, n);
		buf[l+1+n] = 0;
		map = __map_file(buf, &map_size);
		if (map) {
			new = malloc(sizeof *new);
			if (!new) {
				__munmap((void *)map, map_size);
				return 0;
			}
			new->map = map;
			new->map_size = map_size;
			memcpy(new->name, name, n);
			new->name[n] = 0;
			do {
				old_head = loc_head;
				new->next = old_head;
			} while (a_cas_p(&loc_head, old_head, new) != old_head);
			return new;
		}
	}
	return 0;
}

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
		(val = getenv("LC_ALL")) && *val ||
		(val = getenv(envvars[cat])) && *val ||
		(val = getenv("LANG")) && *val ||
		(val = "C.UTF-8");
	}

	size_t n;
	for (n=0; n<LOCALE_NAME_MAX && val[n] && val[n]!='/'; n++);
	if (val[0]=='.' || val[n]) val = "C.UTF-8";
	int builtin = (val[0]=='C' && !val[1])
		|| !strcmp(val, "C.UTF-8")
		|| !strcmp(val, "POSIX");
	struct __locale_map *data, *old;

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
		data = builtin ? 0 : findlocale(val, n);
		if (data == loc->cat[cat-2]) break;
		do old = loc->cat[cat-2];
		while (a_cas_p(&loc->cat[cat-2], old, data) != old);
	case LC_NUMERIC:
		break;
	}
	return 0;
}
