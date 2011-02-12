#undef MAXSYMLINKS
#define MAXSYMLINKS 20

#undef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64

#undef MAXNAMLEN
#define MAXNAMLEN NAME_MAX

#undef MAXPATHLEN
#define MAXPATHLEN PATH_MAX

#include <sys/resource.h>
#include <endian.h>
#include <limits.h>
