#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "libc.h"

/* Ensure that at least 32 atexit handlers can be registered without malloc */
#define COUNT 32

static struct fl
{
	struct fl *next;
	void (*f[COUNT])(void);
} builtin, *head;

static int run_atexit_functions(void)
{
	int i;
	for (; head; head=head->next) {
		for (i=COUNT-1; i>=0 && !head->f[i]; i--);
		for (; i>=0; i--) head->f[i]();
	}
	return 0;
}

int (*const __funcs_on_exit)(void) = run_atexit_functions;

int atexit(void (*func)(void))
{
	static int lock;
	int i;

	/* Hook for atexit extensions */
	if (libc.atexit) return libc.atexit(func);

	LOCK(&lock);

	/* Defer initialization of head so it can be in BSS */
	if (!head) head = &builtin;

	/* If the current function list is full, add a new one */
	if (head->f[COUNT-1]) {
		struct fl *new_fl = calloc(sizeof(struct fl), 1);
		if (!new_fl) {
			UNLOCK(&lock);
			return -1;
		}
		new_fl->next = head;
		head = new_fl;
	}

	/* Append function to the list. */
	for (i=0; i<COUNT && head->f[i]; i++);
	head->f[i] = func;

	UNLOCK(&lock);
	return 0;
}
