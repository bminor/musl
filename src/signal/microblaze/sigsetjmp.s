.global sigsetjmp
.type sigsetjmp,@function
sigsetjmp:
	lwi     r11, r5, 72
	beqi    r11, setjmp
	addi    r7, r5, 76
	add     r6, r0, r0
	brid    sigprocmask
	ori     r5, r0, 2
