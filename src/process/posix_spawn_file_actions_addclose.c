#include <spawn.h>
#include <stdlib.h>
#include <errno.h>
#include "fdop.h"

int posix_spawn_file_actions_addclose(posix_spawn_file_actions_t *fa, int fd)
{
	struct fdop *op = malloc(sizeof *op);
	if (!op) return ENOMEM;
	op->cmd = FDOP_CLOSE;
	op->fd = fd;
	op->next = fa->__actions;
	fa->__actions = op;
	return 0;
}
