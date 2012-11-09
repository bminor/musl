        .global sigsetjmp
        .type sigsetjmp,%function
sigsetjmp:
	#int sigsetjmp(sigjmp_buf buf, int save)
	#		r3		r4
	#0) store save into buf->__fl
	stw 4, 256(3)
	#1) compare save with 0
	cmpwi cr7, 4, 0
	#2) if its 0, goto setjmp code
	beq- cr7, 1f
	#3) else: we must call pthread_sigmask(SIG_SETMASK, 0, (sigset_t *)buf->__ss);
	# thus store r3 on the stack, to restore it later
	stw 3, -4(1)
	# store old link reg
	mflr 0
	stw 0, -8(1)
	# increase stack frame by 16
	subi 1, 1, 16
	# put pointer to ss buf into r5 (3rd arg)
	addi 5, 3, 260
	# put "2" i.e. SIG_SETMASK in r3
	li 3, 2
	li 4, 0
	bl pthread_sigmask
	#restore sp
	addi 1, 1, 16
	#restore r3
	lwz 3, -4(1)
	#restore link reg
	lwz 0, -8(1)
	mtlr 0
1:
	b setjmp
