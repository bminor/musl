#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
#include <pthread.h>
#include "pwf.h"

/* This implementation support Openwall-style TCB passwords in place of
 * traditional shadow, if the appropriate directories and files exist.
 * Thus, it is careful to avoid following symlinks or blocking on fifos
 * which a malicious user might create in place of his or her TCB shadow
 * file. It also avoids any allocation to prevent memory-exhaustion
 * attacks via huge TCB shadow files. */

static long xatol(const char *s)
{
	return isdigit(*s) ? atol(s) : -1;
}

static void cleanup(void *p)
{
	fclose(p);
}

int getspnam_r(const char *name, struct spwd *sp, char *buf, size_t size, struct spwd **res)
{
	char path[20+NAME_MAX];
	FILE *f = 0;
	int rv = 0;
	int fd;
	size_t k, l = strlen(name);
	char *s;
	int skip = 0;
	int cs;

	*res = 0;

	/* Disallow potentially-malicious user names */
	if (*name=='.' || strchr(name, '/') || !l)
		return EINVAL;

	/* Buffer size must at least be able to hold name, plus some.. */
	if (size < l+100) return ERANGE;

	/* Protect against truncation */
	if (snprintf(path, sizeof path, "/etc/tcb/%s/shadow", name) >= sizeof path)
		return EINVAL;

	fd = open(path, O_RDONLY|O_NOFOLLOW|O_NONBLOCK|O_CLOEXEC);
	if (fd >= 0) {
		struct stat st = { 0 };
		errno = EINVAL;
		if (fstat(fd, &st) || !S_ISREG(st.st_mode) || !(f = fdopen(fd, "rb"))) {
			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
			close(fd);
			pthread_setcancelstate(cs, 0);
			return errno;
		}
	} else {
		f = fopen("/etc/shadow", "rbe");
		if (!f) return errno;
	}

	pthread_cleanup_push(cleanup, f);
	while (fgets(buf, size, f) && (k=strlen(buf))>0) {
		if (skip || strncmp(name, buf, l)) {
			skip = buf[k-1] != '\n';
			continue;
		}
		if (buf[k-1] != '\n') {
			rv = ERANGE;
			break;
		}
		buf[k-1] = 0;

		s = buf;
		sp->sp_namp = s;
		if (!(s = strchr(s, ':'))) continue;

		*s++ = 0; sp->sp_pwdp = s;
		if (!(s = strchr(s, ':'))) continue;

		*s++ = 0; sp->sp_lstchg = xatol(s);
		if (!(s = strchr(s, ':'))) continue;

		*s++ = 0; sp->sp_min = xatol(s);
		if (!(s = strchr(s, ':'))) continue;

		*s++ = 0; sp->sp_max = xatol(s);
		if (!(s = strchr(s, ':'))) continue;

		*s++ = 0; sp->sp_warn = xatol(s);
		if (!(s = strchr(s, ':'))) continue;

		*s++ = 0; sp->sp_inact = xatol(s);
		if (!(s = strchr(s, ':'))) continue;

		*s++ = 0; sp->sp_expire = xatol(s);
		if (!(s = strchr(s, ':'))) continue;

		*s++ = 0; sp->sp_flag = xatol(s);
		*res = sp;
		break;
	}
	pthread_cleanup_pop(1);
	return rv;
}
