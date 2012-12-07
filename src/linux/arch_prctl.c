#include "syscall.h"
#ifdef SYS_arch_prctl
#include "libc.h"
int __arch_prctl(int code, unsigned long addr)
{
	return syscall(SYS_arch_prctl, code, addr);
}
weak_alias(__arch_prctl, arch_prctl);
#endif
