static inline struct pthread *__pthread_self()
{
	struct pthread *self;
	__asm__ __volatile__ ("movq %%fs:0,%0" : "=r" (self) );
	return self;
}

#define CANCEL_REG_SP 15
#define CANCEL_REG_IP 16
