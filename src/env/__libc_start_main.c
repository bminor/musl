#include <elf.h>
#include "libc.h"

void __init_tls(size_t *);
void __init_security(size_t *);
void __init_ldso_ctors(void);

#define AUX_CNT 38

extern size_t __hwcap, __sysinfo;

void __init_libc(char **envp)
{
	size_t i, *auxv, aux[AUX_CNT] = { 0 };
	__environ = envp;
	for (i=0; envp[i]; i++);
	libc.auxv = auxv = (void *)(envp+i+1);
	for (i=0; auxv[i]; i+=2) if (auxv[i]<AUX_CNT) aux[auxv[i]] = auxv[i+1];
	__hwcap = aux[AT_HWCAP];
	__sysinfo = aux[AT_SYSINFO];

	__init_tls(aux);
	__init_security(aux);
}

int __libc_start_main(
	int (*main)(int, char **, char **), int argc, char **argv,
	int (*init)(int, char **, char **), void (*fini)(void),
	void (*ldso_fini)(void))
{
	char **envp = argv+argc+1;

	__init_libc(envp);

	libc.ldso_fini = ldso_fini;
	libc.fini = fini;

	/* Execute constructors (static) linked into the application */
	if (init) init(argc, argv, envp);

#ifdef SHARED
	__init_ldso_ctors();
#endif

	/* Pass control to to application */
	exit(main(argc, argv, envp));
	return 0;
}
