typedef pthread_t (*__pthread_self_func_t)(void) __attribute__((const));

#define __pthread_self ((__pthread_self_func_t)0xffff0fe0)

#define CANCEL_REG_SP 16
#define CANCEL_REG_IP 18
