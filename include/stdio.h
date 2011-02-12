#ifndef _STDIO_H
#define _STDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#define __NEED_FILE
#define __NEED_va_list
#define __NEED_size_t
#define __NEED_ssize_t
#define __NEED_off_t

#include <bits/alltypes.h>

#undef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif

#undef EOF
#define EOF (-1)

#undef SEEK_SET
#undef SEEK_CUR
#undef SEEK_END
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#include <bits/stdio.h>

typedef union {
	char __opaque[16];
	double __align;
} fpos_t;

extern FILE *const stdin;
extern FILE *const stdout;
extern FILE *const stderr;

#define stdin  (stdin)
#define stdout (stdout)
#define stderr (stderr)

FILE *fopen(const char *, const char *);
FILE *fdopen(int, const char *);
FILE *freopen(const char *, const char *, FILE *);
int fclose(FILE *);

FILE *popen(const char *, const char *);
int pclose(FILE *);

int remove(const char *);
int rename(const char *, const char *);

int fileno(FILE *);
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
void clearerr(FILE *);

int fseek(FILE *, long, int);
int fseeko(FILE *, off_t, int);
long ftell(FILE *);
off_t ftello(FILE *);
void rewind(FILE *);

int fgetpos(FILE *, fpos_t *);
int fsetpos(FILE *, const fpos_t *);

size_t fread(void *, size_t, size_t, FILE *);
size_t fwrite(const void *, size_t, size_t, FILE *);

int fgetc(FILE *);
int getc(FILE *);
int getchar(void);
int ungetc(int, FILE *);

int fputc(int, FILE *);
int putc(int, FILE *);
int putchar(int);

char *fgets(char *, int, FILE *);
char *gets(char *);

int fputs(const char *, FILE *);
int puts(const char *);

int printf(const char *, ...);
int fprintf(FILE *, const char *, ...);
int sprintf(char *, const char *, ...);
int snprintf(char *, size_t, const char *, ...);

int vprintf(const char *, va_list);
int vfprintf(FILE *, const char *, va_list);
int vsprintf(char *, const char *, va_list);
int vsnprintf(char *, size_t, const char *, va_list);

int dprintf(int, const char *, ...);
int vdprintf(int, const char *, va_list);

int scanf(const char *, ...);
int fscanf(FILE *, const char *, ...);
int sscanf(const char *, const char *, ...);
int vscanf(const char *, va_list);
int vfscanf(FILE *, const char *, va_list);
int vsscanf(const char *, const char *, va_list);

void perror(const char *);

void flockfile(FILE *);
int ftrylockfile(FILE *);
void funlockfile(FILE *);
int getc_unlocked(FILE *);
int getchar_unlocked(void);
int putc_unlocked(int, FILE *);
int putchar_unlocked(int);

int setvbuf(FILE *, char *, int, size_t);
void setbuf(FILE *, char *);

char *tmpnam(char *);
char *tempnam(const char *, const char *);
FILE *tmpfile(void);

char *ctermid(char *);

ssize_t getdelim(char **, size_t *, int, FILE *);
ssize_t getline(char **, size_t *, FILE *);

int renameat(int, const char *, int, const char *);

#ifdef __cplusplus
}
#endif

#endif
