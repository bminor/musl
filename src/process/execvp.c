#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

extern char **__environ;

int execvp(const char *file, char *const argv[])
{
	const char *p, *z, *path = getenv("PATH");
	int l;

	if (strchr(file, '/'))
		return execve(file, argv, __environ);

	/* FIXME: integer overflows */
	if (!path) path = "/usr/local/bin:/bin:/usr/bin";
	l = strlen(file) + strlen(path) + 2;

	for(p=path; p && *p; p=z) {
		char b[l];
		z = strchr(p, ':');
		if (z) {
			memcpy(b, p, z-p);
			b[z++-p] = 0;
		} else strcpy(b, p);
		strcat(b, "/");
		strcat(b, file);
		if (!access(b, X_OK))
			return execve(b, argv, __environ);
	}
	errno = ENOENT;
	return -1;
}
