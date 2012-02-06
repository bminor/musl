.section .init
	tst lr,#1
	moveq pc,lr
	bx lr

.section .fini
	tst lr,#1
	moveq pc,lr
	bx lr
