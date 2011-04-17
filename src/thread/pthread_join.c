#include "pthread_impl.h"

int pthread_join(pthread_t t, void **res)
{
	int tmp = t->tid;
	if (tmp) __timedwait_cp(&t->tid, tmp, 0, 0, 1);
	if (res) *res = t->result;
	if (t->map_base) munmap(t->map_base, t->map_size);
	return 0;
}
