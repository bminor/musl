.section .init
	pop {lr}
	tst lr,#1
	moveq pc,lr
	bx lr

.section .fini
	pop {lr}
	tst lr,#1
	moveq pc,lr
	bx lr
