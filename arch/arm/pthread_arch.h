typedef char *(*__ptr_func_t)(void) __attribute__((const));

#define __pthread_self() \
	((pthread_t)(((__ptr_func_t)0xffff0fe0)()+8-sizeof(struct pthread)))

#define TLS_ABOVE_TP
#define TP_ADJ(p) ((char *)(p) + sizeof(struct pthread) - 8)

#define CANCEL_REG_IP 18
