#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int shm_unlink(const char *name)
{
	int dir, ret;

	while (*name == '/') name++;
	if (strchr(name, '/')) {
		errno = EINVAL;
		return -1;
	}

	if ((dir = open("/dev/shm", O_DIRECTORY|O_RDONLY)) < 0) return -1;
	ret = unlinkat(dir, name, 0);
	close(dir);
	return ret;
}
