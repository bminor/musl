#ifndef _STDIO_IMPL_H
#define _STDIO_IMPL_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>
#include <wchar.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/wait.h>
#include <math.h>
#include <float.h>
#include "syscall.h"
#include "libc.h"

#define UNGET 4

#define FLOCK(f) LOCK(&f->lock)
#define FUNLOCK(f) UNLOCK(&f->lock)

#define F_PERM 1
#define F_NORD 4
#define F_NOWR 8
#define F_EOF 16
#define F_ERR 32

struct __FILE_s {
	unsigned flags;
	unsigned char *rpos, *rstop;
	unsigned char *rend, *wend;
	unsigned char *wpos, *wstop;
	unsigned char *wbase;
	unsigned char *dummy01[3];
	unsigned char *buf;
	size_t buf_size;
	FILE *prev, *next;
	int fd;
	int pipe_pid;
	long dummy2;
	short dummy3;
	char dummy4;
	signed char lbf;
	int lock;
	int lockcount;
	void *owner;
	off_t off;
	int (*flush)(FILE *);
	void **wide_data; /* must be NULL */
	size_t (*read)(FILE *, unsigned char *, size_t);
	size_t (*write)(FILE *, const unsigned char *, size_t);
	off_t (*seek)(FILE *, off_t, int);
	int mode;
	int (*close)(FILE *);
};

size_t __stdio_read(FILE *, unsigned char *, size_t);
size_t __stdio_write(FILE *, const unsigned char *, size_t);
off_t __stdio_seek(FILE *, off_t, int);
int __stdio_close(FILE *);

int __overflow(FILE *, int);
int __oflow(FILE *);
int __uflow(FILE *);
int __underflow(FILE *);

int __fseeko(FILE *, off_t, int);
int __fseeko_unlocked(FILE *, off_t, int);
off_t __ftello(FILE *);
off_t __ftello_unlocked(FILE *);
size_t __fwritex(const unsigned char *, size_t, FILE *);
int __putc_unlocked(int, FILE *);

FILE *__fdopen(int, const char *);

extern struct ofl
{
	FILE *head;
	int lock;
} __ofl;

#define OFLLOCK() LOCK(&__ofl.lock)
#define OFLUNLOCK() UNLOCK(&__ofl.lock)
#define ofl_head (__ofl.head)

#define feof(f) ((f)->flags & F_EOF)
#define ferror(f) ((f)->flags & F_ERR)

/* Caller-allocated FILE * operations */
FILE *__fopen_rb_ca(const char *, FILE *, unsigned char *, size_t);
int __fclose_ca(FILE *);

#endif
