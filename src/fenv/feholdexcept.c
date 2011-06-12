#include <fenv.h>

int feholdexcept(fenv_t *envp)
{
	fegetenv(envp);
	return 0;
}
