static inline struct pthread *__pthread_self()
{
	struct pthread *self;
	__asm__ __volatile__ ("movl %%gs:0,%0" : "=r" (self) );
	return self;
}

#define CANCEL_REG_SP 7
#define CANCEL_REG_IP 14
