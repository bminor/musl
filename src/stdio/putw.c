#include <stdio.h>

int putw(int x, FILE *f)
{
	return fwrite(&x, sizeof x, 1, f) ? x : EOF;
}
