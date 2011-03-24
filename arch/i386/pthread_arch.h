static inline struct pthread *__pthread_self()
{
	struct pthread *self;
	__asm__ ("movl %%gs:0,%0" : "=r" (self) );
	return self;
}

#define PC_AT_SYS(c) \
	(*(uint16_t *)(((ucontext_t *)(c))->uc_mcontext.__gregs[14])==0x80cd)
