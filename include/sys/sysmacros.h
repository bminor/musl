#ifndef _SYS_SYSMACROS_H
#define _SYS_SYSMACROS_H

#define major(x) (((x) >> 8) & 0xff)
#define minor(x) ((x) & 0xff)
#define makedev(x,y) (((x)<<8)|((y)&0xff))

#endif
