#include <math.h>
#include "libm.h"
#include "libc.h"

int __signgam = 0;

weak_alias(__signgam, signgam);
