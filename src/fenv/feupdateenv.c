#include <fenv.h>

int feupdateenv(const fenv_t *envp)
{
	#pragma STDC FENV_ACCESS ON
	int ex = fetestexcept(FE_ALL_EXCEPT);
	fesetenv(envp);
	feraiseexcept(ex);
	return 0;
}
