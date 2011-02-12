#define major(x) (((x) >> 8) & 0xff)
#define minor(x) ((x) & 0xff)
#define makedev(x,y) (((x)<<8)|((y)&0xff))

//#define makedev(x,y) \
//	((x)*0x100000001ULL)&(0xfffffffffff0)
//	((y)*0x1001 & 0xffff0ff)
