#include <semaphore.h>
#include <sys/mman.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>

static void *find_map(int fd)
{
	char c;
	struct stat st;
	FILE *f;
	void *addr;
	unsigned long long off, ino;
	char buf[100];

	if (fstat(fd, &st) < 0) return 0;
	if (!(f = fopen("/proc/self/maps", "rb"))) return 0;
	
	while (fgets(buf, sizeof buf, f)) {
		sscanf(buf, "%lx-%*lx %*s %llx %*x:%*x %llu /dev/shm%c",
			(long *)&addr, &off, &ino, &c);
		while (!strchr(buf, '\n') && fgets(buf, sizeof buf, f));
		if (c!='/') continue;
		c = 0;
		if (!off && st.st_ino == ino) {
			fclose(f);
			return addr;
		}
	}
	fclose(f);
	return 0;
}

sem_t *sem_open(const char *name, int flags, ...)
{
	va_list ap;
	mode_t mode;
	unsigned value;
	int fd, tfd, dir;
	sem_t newsem;
	void *map;
	char tmp[64];
	struct timespec ts;

	while (*name=='/') name++;
	if (strchr(name, '/')) {
		errno = EINVAL;
		return SEM_FAILED;
	}

	if (flags & O_CREAT) {
		va_start(ap, flags);
		mode = va_arg(ap, mode_t) & 0666;
		value = va_arg(ap, unsigned);
		va_end(ap);
		if (value > SEM_VALUE_MAX) {
			errno = EINVAL;
			return SEM_FAILED;
		}
		sem_init(&newsem, 0, value);
		clock_gettime(CLOCK_REALTIME, &ts);
		snprintf(tmp, sizeof(tmp), "/dev/shm/%p-%p-%d-%d",
			&name, name, (int)getpid(), (int)ts.tv_nsec);
		tfd = open(tmp, O_CREAT|O_EXCL|O_RDWR, mode);
		if (tfd<0) return SEM_FAILED;
		dir = open("/dev/shm", O_DIRECTORY|O_RDONLY);
		if (dir<0 || write(tfd,&newsem,sizeof newsem)!=sizeof newsem) {
			if (dir >= 0) close(dir);
			close(tfd);
			unlink(tmp);
			return SEM_FAILED;
		}
	}

	flags &= ~O_ACCMODE;
	flags |= O_RDWR;

	for (;;) {
		if (!(flags & O_EXCL)) {
			fd = shm_open(name, flags&~O_CREAT, mode);
			if (fd >= 0 || errno != ENOENT) {
				if (flags & O_CREAT) {
					close(dir);
					close(tfd);
					unlink(tmp);
				}
				if (fd < 0) return SEM_FAILED;
				if ((map = find_map(fd)))
					return map;
				break;
			}
		}
		if (!linkat(AT_FDCWD, tmp, dir, name, 0)) {
			fd = tfd;
			close(dir);
			unlink(tmp);
			break;
		}
		if ((flags & O_EXCL) || errno != EEXIST) {
			close(dir);
			close(tfd);
			unlink(tmp);
			return SEM_FAILED;
		}
	}
	map = mmap(0, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);
	if (map == MAP_FAILED) return SEM_FAILED;
	return map;
}
