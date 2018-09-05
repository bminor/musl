#include "version.h"
#include "libc.h"

static const char version[] = VERSION;

hidden const char *__libc_get_version()
{
	return version;
}
