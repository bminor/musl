#include <sys/stat.h>
#include "syscall.h"

int mknod(const char *path, mode_t mode, dev_t dev)
{
	/* since dev_t is system-specific anyway we defer to the idiotic
	 * legacy-compatible bitfield mapping of the type.. at least we've
	 * made it large enough to leave space for future expansion.. */
	return syscall(SYS_mknod, path, mode, dev & 0xffff);
}
