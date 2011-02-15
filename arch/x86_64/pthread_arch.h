static inline struct pthread *__pthread_self()
{
	struct pthread *self;
	__asm__ ("movq %%fs:0,%0" : "=r" (self) );
	return self;
}
