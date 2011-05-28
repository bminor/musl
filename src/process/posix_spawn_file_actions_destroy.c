#include <spawn.h>
#include <stdlib.h>

int posix_spawn_file_actions_destroy(posix_spawn_file_actions_t *fa)
{
	// FIXME
	free(fa->__actions);
	return 0;
}
