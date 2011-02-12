#include <signal.h>
#include <string.h>

int sigemptyset(sigset_t *set)
{
	memset(set, 0, sizeof *set);
	return 0;
}
