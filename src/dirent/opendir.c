#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "__dirent.h"

DIR *opendir(const char *name)
{
	int fd;
	DIR *dir;

	if ((fd = open(name, O_RDONLY|O_DIRECTORY)) < 0)
		return 0;
	fcntl(fd, F_SETFD, FD_CLOEXEC);
	if (!(dir = calloc(1, sizeof *dir))) {
		close(fd);
		return 0;
	}
	dir->fd = fd;
	return dir;
}
