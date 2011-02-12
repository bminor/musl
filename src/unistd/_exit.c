#include <unistd.h>
#include <stdlib.h>

void _exit(int status)
{
	_Exit(status);
}
