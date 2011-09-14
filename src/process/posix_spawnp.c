#include <spawn.h>
#include <unistd.h>

int __posix_spawnx(pid_t *, const char *,
	int (*)(const char *, char *const *),
	const posix_spawn_file_actions_t *,
	const posix_spawnattr_t *, char *const [], char *const []);

int posix_spawnp(pid_t *res, const char *file,
	const posix_spawn_file_actions_t *fa,
	const posix_spawnattr_t *attr,
	char *const argv[], char *const envp[])
{
	return __posix_spawnx(res, file, execvp, fa, attr, argv, envp);
}
