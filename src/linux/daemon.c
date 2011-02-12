#include <fcntl.h>
#include <unistd.h>

int daemon(int nochdir, int noclose)
{
	int fd;

	switch(fork()) {
	case 0: break;
	case -1: return -1;
	default: _exit(0);
	}

	if (setsid() < 0) return -1;

	switch(fork()) {
	case 0: break;
	case -1: return -1;
	default: _exit(0);
	}

	if (!nochdir) chdir("/");
	if (!noclose && (fd = open("/dev/null", O_RDWR)) >= 0) {
		dup2(fd, 0);
		dup2(fd, 1);
		dup2(fd, 2);
		if (fd > 2) close(fd);
	}

	return 0;
}
