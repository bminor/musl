static inline struct pthread *__pthread_self()
{
	struct pthread *self;
	__asm__ ("movq %%fs:0,%0" : "=r" (self) );
	return self;
}

#define PC_AT_SYS(c) \
	(*(uint16_t *)(((ucontext_t *)(c))->uc_mcontext.__gregs[16])==0x050f)
