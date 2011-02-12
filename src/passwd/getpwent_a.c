#include "pwf.h"

struct passwd *__getpwent_a(FILE *f, struct passwd *pw, char **line, size_t *size)
{
	ssize_t l;
	char *s;
	for (;;) {
		if ((l=getline(line, size, f)) < 0) {
			free(*line);
			*line = 0;
			return 0;
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
		return pw;
	}
}
