static inline struct pthread *__pthread_self()
{
	struct pthread *self;
	__asm__ __volatile__ (".word 0x7c03e83b" : "=v" (self) );
	return self;
}

#define CANCEL_REG_IP (3-(union {int __i; char __b;}){1}.__b)
