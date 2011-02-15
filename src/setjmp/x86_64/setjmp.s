/* Copyright 2011 Nicholas J. Kain, licensed GNU LGPL 2.1 or later */
.global __setjmp
.global _setjmp
.global setjmp
.type __setjmp,%function
.type _setjmp,%function
.type setjmp,%function
__setjmp:
_setjmp:
setjmp:
	mov %rbx,(%rdi)         /* rdi is jmp_buf, move registers onto it */
	mov %rbp,8(%rdi)
	mov %r12,16(%rdi)
	mov %r13,24(%rdi)
	mov %r14,32(%rdi)
	mov %r15,40(%rdi)
	leaq 8(%rsp),%rdx       /* this is our rsp WITHOUT current ret addr */
	mov %rdx,48(%rdi)
	movq (%rsp),%rdx        /* save return addr ptr for new rip */
	mov %rdx,56(%rdi)
	xor %rax,%rax           /* always return 0 */
	ret
.size __setjmp,.-__setjmp
.size _setjmp,.-_setjmp
.size setjmp,.-setjmp
