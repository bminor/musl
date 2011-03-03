#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int shm_open(const char *name, int flag, mode_t mode)
{
	int fd, dir;

	while (*name == '/') name++;
	if (strchr(name, '/')) {
		errno = EINVAL;
		return -1;
	}

	if ((dir = open("/dev/shm", O_DIRECTORY|O_RDONLY)) < 0) return -1;
	fd = openat(dir, name, flag|O_NOFOLLOW|O_CLOEXEC|O_NONBLOCK, mode);
	close(dir);
	return fd;
}
