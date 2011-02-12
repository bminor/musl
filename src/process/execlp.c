#include <unistd.h>
#include <stdarg.h>

int execlp(const char *file, ...)
{
	int argc;
	va_list ap;
	va_start(ap, file);
	for (argc=0; va_arg(ap, const char *); argc++);
	va_end(ap);
	{
		int i;
		char *argv[argc+1];
		va_start(ap, file);
		for (i=0; i<argc; i++)
			argv[i] = va_arg(ap, char *);
		argv[i] = NULL;
		return execvp(file, argv);
	}
}
