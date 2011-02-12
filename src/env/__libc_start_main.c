#include "libc.h"

/* Any use of __environ/environ will override this symbol. */
char **__dummy_environ = (void *)-1;
weak_alias(__dummy_environ, ___environ);

int __libc_start_main(
	int (*main)(int, char **, char **), int argc, char **argv,
	int (*init)(int, char **, char **), void (*fini)(void),
	void (*ldso_fini)(void))
{
	/* Save the environment if it may be used by libc/application */
	char **envp = argv+argc+1;
	if (___environ != (void *)-1) ___environ = envp;

	/* Avoid writing 0 and triggering unnecessary COW */
	if (ldso_fini) libc.ldso_fini = ldso_fini;
	if (fini) libc.fini = fini;

	/* Execute constructors (static) linked into the application */
	if (init) init(argc, argv, envp);

	/* Pass control to to application */
	exit(main(argc, argv, envp));
	return 0;
}
