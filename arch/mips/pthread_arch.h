static inline struct pthread *__pthread_self()
{
	struct pthread *self;
	__asm__ __volatile__ (".word 0x7c03e83b" : "=v" (self) );
	return self;
}

#define CANCEL_REG_IP 3
