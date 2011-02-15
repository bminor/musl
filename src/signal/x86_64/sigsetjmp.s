/* Copyright 2011 Nicholas J. Kain, licensed GNU LGPL 2.1 or later */
.global sigsetjmp
sigsetjmp:
	test %rsi,%rsi
	jz 1f              /* if save == 0, just goto setjmp */
	movq %rsi,64(%rdi) /* move save -> jmp_buf[8] */
	addq $72,%rdi      /* add sizeof(jmp_buf) to rdi */
	movl $0,%esi       /* arg2 = 0 */
	movl $2,%edx       /* arg3 = 2 */
	call sigprocmask   /* sigprocmask(jmp_buf, 0, 2) */
1:	jmp setjmp
