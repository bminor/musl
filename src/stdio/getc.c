#include "stdio_impl.h"

int getc(FILE *f)
{
	return fgetc(f);
}
