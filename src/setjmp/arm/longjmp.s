.global _longjmp
.global longjmp
.type _longjmp,%function
.type longjmp,%function
_longjmp:
longjmp:
	mov ip,r0
	movs r0,r1
	moveq r0,#1
	ldmia ip!, {v1,v2,v3,v4,v5,v6,sl,fp}
	ldr sp,[ip],#4
	ldr lr,[ip],#4
	tst lr,#1
	moveq pc,lr
	bx lr
