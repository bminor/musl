#include <signal.h>

void (*bsd_signal(int sig, void (*func)(int)))(int)
{
	return signal(sig, func);
}
