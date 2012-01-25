#undef MAXSYMLINKS
#define MAXSYMLINKS 20

#undef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64

#undef MAXNAMLEN
#define MAXNAMLEN 255

#undef MAXPATHLEN
#define MAXPATHLEN 4096

#undef MIN
#undef MAX
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#include <sys/resource.h>
#include <endian.h>
#include <limits.h>
