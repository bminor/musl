static inline struct pthread *__pthread_self()
{
	struct pthread *self;
	__asm__ ("movl %%gs:0,%0" : "=r" (self) );
	return self;
}
