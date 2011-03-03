#include "pthread_impl.h"

static struct pthread main_thread;

/* pthread_key_create.c overrides this */
static const size_t dummy = 0;
weak_alias(dummy, __pthread_tsd_size);

#undef errno
static int *errno_location()
{
	return __pthread_self()->errno_ptr;
}

static int init_main_thread()
{
	void *tsd = 0;
	if (__pthread_tsd_size) {
		tsd = mmap(0, __pthread_tsd_size, PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		if (tsd == MAP_FAILED) return -1;
	}
	main_thread.tsd = tsd;
	main_thread.self = &main_thread;
	if (__set_thread_area(&main_thread) < 0)
		return -1;
	syscall1(__NR_set_tid_address, (long)&main_thread.tid);
	main_thread.errno_ptr = __errno_location();
	libc.errno_location = errno_location;
	main_thread.tid = main_thread.pid = getpid();
	return 0;
}

pthread_t pthread_self()
{
	static int init, failed;
	if (!init) {
		if (failed) return 0;
		if (init_main_thread() < 0) failed = 1;
		if (failed) return 0;
		init = 1;
	}
	return __pthread_self();
}
