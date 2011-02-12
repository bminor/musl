#ifndef _STDLIB_H
#define _STDLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#undef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif

#define __NEED_size_t
#define __NEED_wchar_t

#include <bits/alltypes.h>

int atoi (const char *);
long atol (const char *);
long long atoll (const char *);
double atof (const char *);

float strtof (const char *, char **);
double strtod (const char *, char **);
long double strtold (const char *, char **);

long strtol (const char *, char **, int);
unsigned long strtoul (const char *, char **, int);

long long strtoll (const char *, char **, int);
unsigned long long strtoull (const char *, char **, int);

char *l64a (long);
long a64l (const char *);

long int random (void);
void srandom (unsigned int);
char *initstate (unsigned int, char *, size_t);
char *setstate (char *);

int rand (void);
void srand (unsigned);
int rand_r (unsigned *);

double drand48 (void);
double erand48 (unsigned short [3]);
long int lrand48 (void);
long int nrand48 (unsigned short [3]);
long mrand48 (void);
long jrand48 (unsigned short [3]);
void srand48 (long);
unsigned short *seed48 (unsigned short [3]);
void lcong48 (unsigned short [7]);

void *malloc (size_t);
void *calloc (size_t, size_t);
void *realloc (void *, size_t);
void free (void *);
void *valloc (size_t);
int posix_memalign (void **, size_t, size_t);

void abort (void);
int atexit (void (*) (void));
void exit (int);
void _Exit (int);


char *getenv (const char *);
int putenv (char *);
int setenv (const char *, const char *, int);
int unsetenv (const char *);


char *mktemp (char *);
int mkstemp (char *);

int system (const char *);


char *realpath (const char *, char *);

void *bsearch (const void *, const void *, size_t, size_t, int (*)(const void *, const void *));
void qsort (void *, size_t, size_t, int (*)(const void *, const void *));

int abs (int);
long labs (long);
long long llabs (long long);

typedef struct { int quot, rem; } div_t;
extern div_t div (int, int);

typedef struct { long quot, rem; } ldiv_t;
ldiv_t ldiv (long, long);

typedef struct { long long quot, rem; } lldiv_t;
lldiv_t lldiv (long long, long long);


int mblen (const char *, size_t);
int mbtowc (wchar_t *, const char *, size_t);
int wctomb (char *, wchar_t);
size_t mbstowcs (wchar_t *, const char *, size_t);
size_t wcstombs (char *, const wchar_t *, size_t);

int getsubopt (char **, char *const *, char **);

void setkey (const char *);

int posix_openpt (int);
int grantpt (int);
int unlockpt (int);
char *ptsname (int);

#define MB_CUR_MAX 4

#define RAND_MAX (0x7fffffff)

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#include <bits/wexitstatus.h>

#ifdef __cplusplus
}
#endif

#endif
