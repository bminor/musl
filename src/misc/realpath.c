#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

char *realpath(const char *restrict filename, char *restrict resolved)
{
	int fd;
	ssize_t r;
	struct stat st1, st2;
	char buf[15+3*sizeof(int)];
	int alloc = 0;

	if (!filename) {
		errno = EINVAL;
		return 0;
	}

	fd = open(filename, O_RDONLY|O_NONBLOCK|O_CLOEXEC);
	if (fd < 0) return 0;
	snprintf(buf, sizeof buf, "/proc/self/fd/%d", fd);

	if (!resolved) {
		alloc = 1;
		resolved = malloc(PATH_MAX);
		if (!resolved) return 0;
	}

	r = readlink(buf, resolved, PATH_MAX-1);
	if (r < 0) goto err;
	resolved[r] = 0;

	fstat(fd, &st1);
	r = stat(resolved, &st2);
	if (r<0 || st1.st_dev != st2.st_dev || st1.st_ino != st2.st_ino) {
		if (!r) errno = ELOOP;
		goto err;
	}

	close(fd);
	return resolved;
err:
	if (alloc) free(resolved);
	close(fd);
	return 0;
}
