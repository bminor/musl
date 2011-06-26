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
#include <stdlib.h>
#include <pthread.h>

static struct {
	ino_t ino;
	sem_t *sem;
	int refcnt;
} *semtab;

static int semcnt;
static pthread_spinlock_t lock;
static pthread_once_t once;

static void init()
{
	semtab = calloc(sizeof *semtab, SEM_NSEMS_MAX);
}

static sem_t *find_map(ino_t ino)
{
	int i;
	for (i=0; i<SEM_NSEMS_MAX && semtab[i].ino != ino; i++);
	if (i==SEM_NSEMS_MAX) return 0;
	if (semtab[i].refcnt == INT_MAX) return (sem_t *)-1;
	semtab[i].refcnt++;
	return semtab[i].sem;
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
	struct stat st;
	int i;

	while (*name=='/') name++;
	if (strchr(name, '/')) {
		errno = EINVAL;
		return SEM_FAILED;
	}

	pthread_once(&once, init);
	if (!semtab) {
		errno = ENOMEM;
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

	pthread_spin_lock(&lock);

	for (;;) {
		if (!(flags & O_EXCL)) {
			fd = shm_open(name, flags&~O_CREAT, 0);
			if (fd >= 0 || errno != ENOENT) {
				if (flags & O_CREAT) {
					close(dir);
					close(tfd);
					unlink(tmp);
				}
				if (fd >= 0 && fstat(fd, &st) < 0) {
					close(fd);
					fd = -1;
				}
				if (fd < 0) {
					pthread_spin_unlock(&lock);
					return SEM_FAILED;
				}
				if ((map = find_map(st.st_ino))) {
					pthread_spin_unlock(&lock);
					close(fd);
					if (map == (sem_t *)-1)
						return SEM_FAILED;
					return map;
				}
				break;
			}
		}
		if (!(flags & O_CREAT)) {
			pthread_spin_unlock(&lock);
			return SEM_FAILED;
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
	if (fstat(fd, &st) < 0) {
		pthread_spin_unlock(&lock);
		close(fd);
		return SEM_FAILED;
	}
	if (semcnt == SEM_NSEMS_MAX) {
		pthread_spin_unlock(&lock);
		close(fd);
		errno = EMFILE;
		return SEM_FAILED;
	}
	for (i=0; i<SEM_NSEMS_MAX && semtab[i].sem; i++);
	map = mmap(0, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);
	if (map == MAP_FAILED) {
		pthread_spin_unlock(&lock);
		return SEM_FAILED;
	}
	semtab[i].ino = st.st_ino;
	semtab[i].sem = map;
	semtab[i].refcnt = 1;
	pthread_spin_unlock(&lock);
	return map;
}

int sem_close(sem_t *sem)
{
	int i;
	pthread_spin_lock(&lock);
	for (i=0; i<SEM_NSEMS_MAX && semtab[i].sem != sem; i++);
	if (!--semtab[i].refcnt) {
		semtab[i].sem = 0;
		semtab[i].ino = 0;
	}
	pthread_spin_unlock(&lock);
	return munmap(sem, sizeof *sem);
}
