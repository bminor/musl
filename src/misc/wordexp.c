#include <wordexp.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

static char *getword(FILE *f)
{
	char *s = 0;
	return getdelim(&s, (size_t [1]){0}, 0, f) < 0 ? 0 : s;
}

static int do_wordexp(const char *s, wordexp_t *we, int flags)
{
	size_t i, l;
	int sq=0, dq=0;
	size_t np=0;
	char *w, **tmp;
	char *redir = (flags & WRDE_SHOWERR) ? "" : "2>/dev/null";
	int err = 0, status;
	FILE *f;
	size_t wc = 0;
	char **wv = 0;
	int p[2];
	pid_t pid;

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
	} else {
		we->we_offs = 0;
	}

	if (pipe(p) < 0) return WRDE_NOSPACE;
	pid = fork();
	if (pid < 0) {
		close(p[0]);
		close(p[1]);
		return WRDE_NOSPACE;
	}
	if (!pid) {
		dup2(p[1], 1);
		close(p[0]);
		close(p[1]);
		execl("/bin/sh", "sh", "-c",
			"eval \"printf %s\\\\\\\\0 $1 $2\"",
			"sh", s, redir, (char *)0);
		_exit(1);
	}
	close(p[1]);
	
	f = fdopen(p[0], "r");
	if (!f) {
		close(p[0]);
		kill(pid, SIGKILL);
		waitpid(pid, &status, 0);
		return WRDE_NOSPACE;
	}

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

	fclose(f);
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status)) {
		if (!(flags & WRDE_APPEND)) {
			free(wv);
			return WRDE_SYNTAX;
		} else if (wv==we->we_wordv) {
			return WRDE_SYNTAX;
		}
	}

	we->we_wordv = wv;
	we->we_wordc = i;

	for (i=we->we_offs; i; i--)
		we->we_wordv[i-1] = 0;

	if (flags & WRDE_DOOFFS) we->we_wordc -= we->we_offs;
	return err;
}

int wordexp(const char *restrict s, wordexp_t *restrict we, int flags)
{
	int r, cs;
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
	r = do_wordexp(s, we, flags);
	pthread_setcancelstate(cs, 0);
	return r;
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
