#include <signal.h>
#include <string.h>
#include <limits.h>

int sigfillset(sigset_t *set)
{
#if ULONG_MAX == 0xffffffff
	set->__bits[0] = 0x7ffffffful;
	set->__bits[1] = 0xfffffffcul;
#else
	set->__bits[0] = 0xfffffffc7ffffffful;
#endif
	return 0;
}
