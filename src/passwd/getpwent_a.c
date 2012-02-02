#include "pwf.h"
#include <pthread.h>

struct passwd *__getpwent_a(FILE *f, struct passwd *pw, char **line, size_t *size)
{
	ssize_t l;
	char *s;
	int cs;
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
	for (;;) {
		if ((l=getline(line, size, f)) < 0) {
			free(*line);
			*line = 0;
			pw = 0;
			break;
		}
		line[0][l-1] = 0;

		s = line[0];
		pw->pw_name = s++;
		if (!(s = strchr(s, ':'))) continue;

		*s++ = 0; pw->pw_passwd = s;
		if (!(s = strchr(s, ':'))) continue;

		*s++ = 0; pw->pw_uid = atoi(s);
		if (!(s = strchr(s, ':'))) continue;

		*s++ = 0; pw->pw_gid = atoi(s);
		if (!(s = strchr(s, ':'))) continue;

		*s++ = 0; pw->pw_gecos = s;
		if (!(s = strchr(s, ':'))) continue;

		*s++ = 0; pw->pw_dir = s;
		if (!(s = strchr(s, ':'))) continue;

		*s++ = 0; pw->pw_shell = s;
		break;
	}
	pthread_setcancelstate(cs, 0);
	return pw;
}
