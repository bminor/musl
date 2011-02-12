#include "pthread_impl.h"

int pthread_join(pthread_t t, void **res)
{
	int tmp = t->tid;
	CANCELPT_BEGIN;
	if (tmp) __wait(&t->tid, 0, tmp, 1);
	CANCELPT_END;
	if (res) *res = t->result;
	if (t->map_base) munmap(t->map_base, t->map_size);
	return 0;
}
