.global sigsetjmp
.type sigsetjmp,@function
sigsetjmp:
	lwi     r11, r5, 72
	beqi    r11, setjmp@PLT
	addi    r7, r5, 76
	add     r6, r0, r0
	brid    sigprocmask@PLT
	ori     r5, r0, 2
