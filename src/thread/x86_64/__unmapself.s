/* Copyright 2011 Nicholas J. Kain, licensed GNU LGPL 2.1 or later */
.text
.global __unmapself
.type   __unmapself,%function
__unmapself:
	call 1f         /* glibc ABI compat */
	.long -1
	.long -1
1:      push %rsi       /* save arg2 for munmap */
	push %rdx       /* save arg3 for munmap */
	mov %rdi,%rsi   /* rt_sigprocmask() args: move arg1 to rsi */
	xor %rdi,%rdi
	xor %rdx,%rdx
	movq $8,%r10
	movl $14,%eax   /* __NR_rt_sigprocmask */
	syscall         /* call rt_sigprocmask(0,arg1,0,8) */
	pop %rsi        /* munmap() args: reload from stack */
	pop %rdi
	movl $11,%eax   /* __NR_munmap */
	syscall         /* munmap(arg2,arg3) */
	xor %rdi,%rdi   /* exit() args: always return success */
	movl $60,%eax   /* __NR_exit */
	syscall         /* exit(0) */
.size __unmapself,.-__unmapself
