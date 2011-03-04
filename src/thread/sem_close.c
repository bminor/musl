#include <semaphore.h>
#include <sys/mman.h>

int sem_close(sem_t *sem)
{
	return munmap(sem, sizeof *sem);
}
