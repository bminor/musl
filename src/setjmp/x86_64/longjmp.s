/* Copyright 2011 Nicholas J. Kain, licensed GNU LGPL 2.1 or later */
.global _longjmp
.global longjmp
.type _longjmp,%function
.type longjmp,%function
_longjmp:
longjmp:
	mov %rsi,%rax           /* val will be longjmp return */
	test %rax,%rax
	jnz .L0
	inc %rax                /* if val==0, val=1 per longjmp semantics */
.L0:
	movq (%rdi),%rbx        /* rdi is the jmp_buf, restore regs from it */
	movq 8(%rdi),%rbp
	movq 16(%rdi),%r12
	movq 24(%rdi),%r13
	movq 32(%rdi),%r14
	movq 40(%rdi),%r15
	movq 48(%rdi),%rdx      /* this ends up being the stack pointer */
	mov %rdx,%rsp
	movq 56(%rdi),%rdx      /* this is the instruction pointer */
	jmp *%rdx               /* goto saved address without altering rsp */
.size _longjmp,.-_longjmp
.size longjmp,.-longjmp
