static inline struct pthread *__pthread_self()
{
	return ((void *(*)(void))0xffff0fe0)();
}

#define CANCEL_REG_SP 16
#define CANCEL_REG_IP 18
