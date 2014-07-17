.global sigsetjmp
.global __sigsetjmp
.type sigsetjmp,@function
.type __sigsetjmp,@function
sigsetjmp:
__sigsetjmp:
	l.sfeq	r4, r0
	l.bf	plt(setjmp)
	 l.sw	52(r3), r4	/* buf->__fl = save */

	l.addi	r1, r1, -8
	l.sw	0(r1), r9
	l.sw	4(r1), r3
	l.addi	r5, r3, 56	/* buf->__ss */
	l.add	r4, r0, r0
	l.jal	plt(sigprocmask)
	 l.ori	r3, r0, 2	/* SIG_SETMASK */

	l.lwz	r9, 0(r1)
	l.lwz	r3, 4(r1)
	l.j	plt(setjmp)
	 l.addi	r1, r1, 8
