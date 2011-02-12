#include <signal.h>
#include <string.h>

int sigfillset(sigset_t *set)
{
	memset(set, -1, sizeof *set);
	return 0;
}
