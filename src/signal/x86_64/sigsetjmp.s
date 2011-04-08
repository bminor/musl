/* Copyright 2011 Nicholas J. Kain, licensed GNU LGPL 2.1 or later */
.global sigsetjmp
sigsetjmp:
	andl %esi,%esi
	movq %rsi,64(%rdi)
	jz 1f
	pushq %rdi
	leaq 72(%rdi),%rsi
	xorl %edx,%edx
	movl $2,%edi
	call sigprocmask
	popq %rdi
1:	jmp setjmp
