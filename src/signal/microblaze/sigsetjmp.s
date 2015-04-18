.global sigsetjmp
.global __sigsetjmp
.type sigsetjmp,@function
.type __sigsetjmp,@function
sigsetjmp:
__sigsetjmp:
	beqi r6, setjmp@PLT

	swi r15,r5,72
	swi r19,r5,72+4+8

	brlid r15,setjmp@PLT
	 ori r19,r5,0

	ori r6,r3,0
	ori r5,r19,0
	lwi r15,r5,72
	lwi r19,r5,72+4+8

.hidden __sigsetjmp_tail
	bri __sigsetjmp_tail
