#if 0

int clone(int (*start)(void *), void *stack, int flags, void *arg,
	pid_t *ptid, struct user_desc *tls, pid_t *ctid)
{
	int ret;
	__asm__(
	        "andl $-16,%%ecx     \n\t"
		"xchgl %%ebx,%2      \n\t"
		"movl %%ebx,(%%ecx)  \n\t"
		"int $0x80           \n\t"
		"testl %%eax,%%eax   \n\t"
		"jnz 1f              \n\t"
		"xorl %%ebp,%%ebp    \n\t"
		"call *%%ebx         \n\t"
		"\n1:                \n\t"
		"xchgl %%ebx,%2      \n\t"
		: "=a" (ret)
		: "a" (__NR_clone), "m" (flags), "c"(stack), "d"(ptid),
		  "S" (tls), "D" (ctid)
		: "memory"
	);
	return __syscall_ret(ret);
}

#endif
