#include <wchar.h>

typedef struct rctx
{
	void (*read)(struct rctx *);
	void *opaque;
	int wide;
	int (*is_space)();
	int l;
	int e;
	int c;
	int u;
	int w;
} rctx_t;

int __scanf(rctx_t *, const wchar_t *, va_list);
