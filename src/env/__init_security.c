#include <stddef.h>
#include <elf.h>
#include <poll.h>
#include <fcntl.h>
#include "syscall.h"
#include "libc.h"
#include "atomic.h"

#define AUX_CNT 24

static void dummy(size_t *auxv)
{
}
weak_alias(dummy, __init_ssp);

void __init_security(size_t *auxv)
{
	size_t i, aux[AUX_CNT] = { 0 };
	struct pollfd pfd[3] = { {.fd=0}, {.fd=1}, {.fd=2} };

#ifndef SHARED
	__init_ssp(auxv);
#endif

	for (; auxv[0]; auxv+=2) if (auxv[0]<AUX_CNT) aux[auxv[0]] = auxv[1];
	__hwcap = aux[AT_HWCAP];
	if (aux[AT_UID]==aux[AT_EUID] && aux[AT_GID]==aux[AT_EGID]
		&& !aux[AT_SECURE]) return;

	__syscall(SYS_poll, pfd, 3, 0);
	for (i=0; i<3; i++)
		if (pfd[i].revents&POLLNVAL)
			if (__syscall(SYS_open, "/dev/null", O_RDWR|O_LARGEFILE)<0)
				a_crash();
	libc.secure = 1;
}
