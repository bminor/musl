#include <unistd.h>
#include <stdarg.h>

int execl(const char *path, ...)
{
	int argc;
	va_list ap;
	va_start(ap, path);
	for (argc=0; va_arg(ap, const char *); argc++);
	va_end(ap);
	{
		int i;
		char *argv[argc+1];
		va_start(ap, path);
		for (i=0; i<argc; i++)
			argv[i] = va_arg(ap, char *);
		argv[i] = NULL;
		return execv(path, argv);
	}
}
