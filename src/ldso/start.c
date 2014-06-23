#include <stdlib.h>

/* stub for archs that lack dynamic linker support */

void _dlstart()
{
	_Exit(1);
}
