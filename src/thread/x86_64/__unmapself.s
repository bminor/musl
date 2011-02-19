/* Copyright 2011 Nicholas J. Kain, licensed GNU LGPL 2.1 or later */
.text
.global __unmapself
.type   __unmapself,%function
__unmapself:
	movl $11,%eax   /* __NR_munmap */
	syscall         /* munmap(arg2,arg3) */
	xor %rdi,%rdi   /* exit() args: always return success */
	movl $60,%eax   /* __NR_exit */
	syscall         /* exit(0) */
.size __unmapself,.-__unmapself
