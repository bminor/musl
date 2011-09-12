#define _GNU_SOURCE
#include <signal.h>
#include <string.h>

int sigisemptyset(const sigset_t *set)
{
	static const sigset_t zeroset;
	return !memcmp(set, &zeroset, 8);
}
