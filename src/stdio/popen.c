#include "stdio_impl.h"
#include "syscall.h"

static inline void nc_close(int fd)
{
	__syscall(SYS_close, fd);
}
#define close(x) nc_close(x)

FILE *popen(const char *cmd, const char *mode)
{
	int p[2];
	int op;
	pid_t pid;
	FILE *f;
	const char *modes = "rw", *mi = strchr(modes, *mode);

	if (mi) {
		op = mi-modes;
	} else {
		errno = EINVAL;
		return 0;
	}
	
	if (pipe(p)) return NULL;
	f = fdopen(p[op], mode);
	if (!f) {
		close(p[0]);
		close(p[1]);
		return NULL;
	}
	
	pid = fork();
	switch (pid) {
	case -1:
		fclose(f);
		close(p[0]);
		close(p[1]);
		return NULL;
	case 0:
		if (dup2(p[1-op], 1-op) < 0) _exit(127);
		if (p[0] != 1-op) close(p[0]);
		if (p[1] != 1-op) close(p[1]);
		execl("/bin/sh", "sh", "-c", cmd, (char *)0);
		_exit(127);
	}
	close(p[1-op]);
	f->pipe_pid = pid;
	return f;
}
