.global __setjmp
.global _setjmp
.global setjmp
.type __setjmp,%function
.type _setjmp,%function
.type setjmp,%function
__setjmp:
_setjmp:
setjmp:
	mov ip,r0
	stmia ip!,{v1,v2,v3,v4,v5,v6,sl,fp}
	mov r2,sp
	stmia ip!,{r2,lr}
	mov r0,#0
	tst lr,#1
	moveq pc,lr
	bx lr
