#include <fnmatch.h>
#include <wctype.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>
#include <limits.h>

static int next(const char **s)
{
	wchar_t c;
	int l = mbtowc(&c, *s, MB_LEN_MAX);
	/* hack to allow literal matches of invalid byte sequences */
	if (l < 0) return (unsigned char)*(*s)++ - 0x100;
	*s += l;
	return c;
}

#define BRACKET_ERROR  -0x100
#define BRACKET_NOCHAR -0x101

static int bracket_next(const char **s)
{
	int c;
	int type;
	if (**s == '[') {
		type = *(*s+1);
		if (type == '.' || type == '=') {
			*s += 2;
			c = next(s);
			if (c <= 0) return BRACKET_ERROR;
			if (**s == type && *(*s+1) == ']') {
				*s += 2;
				return c;
			}
			for (; **s && (**s != type || *(*s+1) != ']'); (*s)++);
			if (!**s) return BRACKET_ERROR;
			*s += 2;
			return BRACKET_NOCHAR;
		}
	}
	c = next(s);
	if (c <= 0) return BRACKET_ERROR;
	return c;
}

#define __FNM_CONT 0x8000

int fnmatch(const char *p, const char *s, int flags)
{
	int c, d, k;
	int not;
	int match;
	int first;
	int no_slash = (flags & FNM_PATHNAME) ? '/' : 0;
	int no_period = (flags & FNM_PERIOD) && !(flags & __FNM_CONT) ? '.' : 0x100;

	flags |= __FNM_CONT;

	while ((c = *p++)) {
		switch (c) {
		case '?':
			k = next(&s);
			if (!k || k == no_period || k == no_slash)
				return FNM_NOMATCH;
			break;
		case '\\':
			if (!(flags & FNM_NOESCAPE)) {
				c = *p++;
				goto literal;
			}
			if (*s++ != c) return FNM_NOMATCH;
			break;
		case '*':
			for (; *p == '*'; p++);
			if (*p && !*s) return FNM_NOMATCH;
			if (*s == no_period)
				return FNM_NOMATCH;
			if (!*p && (!no_slash || !strchr(s, no_slash)))
				return 0;
			for (; *s; s++)
				if (!fnmatch(p, s, flags))
					return 0;
				else if (*s == no_slash)
					break;
			return FNM_NOMATCH;
		case '[':
			not = (*p == '!' || *p == '^');
			if (not) p++;
			k = next(&s);
			if (!k || k == no_slash || k == no_period)
				return FNM_NOMATCH;
			match = 0;
			first = 1;
			for (;;) {
				if (!*p) return FNM_NOMATCH;
				if (*p == ']' && !first) break;
				first = 0;
				if (*p == '[' && *(p+1) == ':') {
					const char *z;
					p += 2;
					for (z=p; *z && (*z != ':' || *(z+1) != ']'); z++);
					if (!*z || z-p > 32) { /* FIXME: symbolic const? */
						return FNM_NOMATCH;
					} else {
						char class[z-p+1];
						memcpy(class, p, z-p);
						class[z-p] = 0;
						if (iswctype(k, wctype(class)))
							match = 1;
					}
					p = z+2;
					continue;
				}
				c = bracket_next(&p);
				if (c == BRACKET_ERROR)
					return FNM_NOMATCH;
				if (c == BRACKET_NOCHAR)
					continue;
				if (*p == '-' && *(p+1) != ']') {
					p++;
					d = bracket_next(&p);
					if (d == BRACKET_ERROR)
						return FNM_NOMATCH;
					if (d == BRACKET_NOCHAR)
						continue;
					if (k >= c && k <= d)
						match = 1;
					continue;
				}
				if (k == c) match = 1;
			}
			p++;
			if (not == match)
				return FNM_NOMATCH;
			break;
		default:
		literal:
			if (*s++ != c)
				return FNM_NOMATCH;
			if (c == no_slash && (flags & FNM_PERIOD)) {
				no_period = '.';
				continue;
			}
			break;
		}
		no_period = 0x100;
	}
	if (*s) return FNM_NOMATCH;
	return 0;
}
