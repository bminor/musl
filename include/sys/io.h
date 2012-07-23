#ifndef	_SYS_IO_H
#define	_SYS_IO_H
#ifdef __cplusplus
extern "C" {
#endif

int ioperm(unsigned long, unsigned long, int);
int iopl(int);

#ifdef __cplusplus
}
#endif
#endif
