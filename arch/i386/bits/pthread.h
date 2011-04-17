struct __ptcb {
	long __jb[7];
	int __dummy;
	struct __ptcb *__next;
	void *__ptrs[3];
};

static inline void __pthread_register_cancel_2(struct __ptcb *__cb)
{
	__asm__ __volatile__( "call __pthread_register_cancel" : : "a"(__cb) : "ecx", "edx", "memory" );
}

static inline void __pthread_unregister_cancel_2(struct __ptcb *__cb)
{
	__asm__ __volatile__( "call __pthread_unregister_cancel" : : "a"(__cb) : "ecx", "edx", "memory" );
}

static inline void __pthread_unwind_next_2(struct __ptcb *__cb)
{
	__asm__ __volatile__( "call __pthread_unwind_next" : : "a"(__cb) : "ecx", "edx", "memory" );
}

#define __pthread_register_cancel __pthread_register_cancel_2
#define __pthread_unregister_cancel __pthread_unregister_cancel_2
#define __pthread_unwind_next __pthread_unwind_next_2
