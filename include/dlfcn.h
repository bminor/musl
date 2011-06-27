#ifndef	_DLFCN_H
#define	_DLFCN_H

#define RTLD_LAZY   1
#define RTLD_NOW    2
#define RTLD_GLOBAL 256
#define RTLD_LOCAL  0

#if 1
#define RTLD_NEXT    ((void *) -1l)
#define RTLD_DEFAULT ((void *) 0)
#endif

int    dlclose(void *);
char  *dlerror(void);
void  *dlopen(const char *, int);
void  *dlsym(void *, const char *);

#endif
