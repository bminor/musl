#include <wordexp.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

static char *getword(FILE *f)
{
	char *s = 0;
	return getdelim(&s, (size_t [1]){0}, 0, f) < 0 ? 0 : s;
}

int wordexp(const char *s, wordexp_t *we, int flags)
{
	size_t i, l, len;
	int sq=0, dq=0;
	size_t np=0;
	char *cmd, *w, **tmp;
	char *redir = (flags & WRDE_SHOWERR) ? "" : "2>/dev/null";
	int err = 0, status;
	FILE *f;
	size_t wc = 0;
	char **wv = 0;

	if (flags & WRDE_REUSE) wordfree(we);

	if (flags & WRDE_NOCMD) for (i=0; s[i]; i++) switch (s[i]) {
	case '\\':
		if (!sq) i++;
		break;
	case '\'':
		if (!dq) sq^=1;
		break;
	case '"':
		if (!sq) dq^=1;
		break;
	case '(':
		if (np) {
			np++;
			break;
		}
	case ')':
		if (np) {
			np--;
			break;
		}
	case '\n':
	case '|':
	case '&':
	case ';':
	case '<':
	case '>':
	case '{':
	case '}':
		if (!(sq|dq|np)) return WRDE_BADCHAR;
		break;
	case '$':
		if (s[i+1]=='(' && s[i+2]=='(') {
			i += 2;
			np += 2;
			break;
		} else if (s[i+1] != '(') break;
	case '`':
		if (sq) break;
		return WRDE_CMDSUB;
	}

	if (flags & WRDE_APPEND) {
		wc = we->we_wordc;
		wv = we->we_wordv;
	}

	i = wc;
	if (flags & WRDE_DOOFFS) {
		if (we->we_offs > SIZE_MAX/sizeof(void *)/4)
			return WRDE_NOSPACE;
		i += we->we_offs;
	}

	len = 50 + strlen(s);
	cmd = malloc(len);
	if (!cmd) return WRDE_NOSPACE;
	snprintf(cmd, len, "printf %%s\\\\0 %s %s", s, redir);
	f = popen(cmd, "r");
	free(cmd);
	if (!f) return WRDE_NOSPACE;

	l = wv ? i+1 : 0;

	while ((w = getword(f))) {
		if (i+1 >= l) {
			l += l/2+10;
			tmp = realloc(wv, l*sizeof(char *));
			if (!tmp) break;
			wv = tmp;
		}
		wv[i++] = w;
		wv[i] = 0;
	}
	if (!feof(f)) err = WRDE_NOSPACE;

	status = pclose(f);
	if (WEXITSTATUS(status)) {
		if (!(flags & WRDE_APPEND)) {
			free(wv);
			return WRDE_SYNTAX;
		} else if (wv==we->we_wordv) {
			return WRDE_SYNTAX;
		}
	}

	we->we_wordv = wv;
	we->we_wordc = i - we->we_offs;
	return err;
}

void wordfree(wordexp_t *we)
{
	size_t i;
	if (!we->we_wordv) return;
	for (i=0; i<we->we_wordc; i++) free(we->we_wordv[we->we_offs+i]);
	free(we->we_wordv);
	we->we_wordv = 0;
	we->we_wordc = 0;
}
