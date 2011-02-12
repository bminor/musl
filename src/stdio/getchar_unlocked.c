#include "stdio_impl.h"

int getchar_unlocked(void)
{
	return stdin->rpos < stdin->rstop ? *stdin->rpos++ : __uflow(stdin);
}
