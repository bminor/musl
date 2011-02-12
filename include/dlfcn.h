#ifndef	_DLFCN_H
#define	_DLFCN_H

#define RTLD_LAZY   0x10000
#define RTLD_NOW    0x20000
#define RTLD_GLOBAL 0x40000
#define RTLD_LOCAL  0x80000

#if 1
#define RTLD_NEXT    ((void *) -1l)
#define RTLD_DEFAULT ((void *) 0)
#endif

int    dlclose(void *);
char  *dlerror(void);
void  *dlopen(const char *, int);
void  *dlsym(void *, const char *);

#endif
