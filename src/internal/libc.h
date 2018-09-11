#ifndef LIBC_H
#define LIBC_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

struct __locale_map;

struct __locale_struct {
	const struct __locale_map *volatile cat[6];
};

struct tls_module {
	struct tls_module *next;
	void *image;
	size_t len, size, align, offset;
};

struct __libc {
	int can_do_threads;
	int threaded;
	int secure;
	volatile int threads_minus_1;
	size_t *auxv;
	struct tls_module *tls_head;
	size_t tls_size, tls_align, tls_cnt;
	size_t page_size;
	struct __locale_struct global_locale;
};

#ifndef PAGE_SIZE
#define PAGE_SIZE libc.page_size
#endif

extern hidden struct __libc __libc;
#define libc __libc

void __init_libc(char **, char *);
void __init_tls(size_t *);
void __libc_start_init(void);

extern hidden size_t __hwcap;
extern hidden size_t __sysinfo;
extern char *__progname, *__progname_full;

extern hidden const char __libc_version[];

/* Designed to avoid any overhead in non-threaded processes */
hidden void __lock(volatile int *);
hidden void __unlock(volatile int *);
hidden int __lockfile(FILE *);
hidden void __unlockfile(FILE *);
#define LOCK(x) __lock(x)
#define UNLOCK(x) __unlock(x)

void __synccall(void (*)(void *), void *);
int __setxid(int, int, int, int);

#undef LFS64_2
#define LFS64_2(x, y) weak_alias(x, y)

#undef LFS64
#define LFS64(x) LFS64_2(x, x##64)

#endif
