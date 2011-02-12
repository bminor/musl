#include "libc.h"

#undef environ
char **___environ = 0;
weak_alias(___environ, __environ);
weak_alias(___environ, _environ);
weak_alias(___environ, environ);
