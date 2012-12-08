.weak __init_array_start
.weak __init_array_end

.section .init
	adr lr, 1f
	ldr r4, 2f
	ldr r5, 2f+4
	add r4, r4, lr
	add r5, r5, lr
1:	adr lr, 1b
	cmp r4, r5
	beq 3f
	ldmia r4!, {r3}
	ldm sp, {r0,r1,r2}
	tst r3,#1
	moveq pc,r3
	bx r3
3:	pop {r0,r1,r2,r4,r5,lr}
	tst lr,#1
	moveq pc,lr
	bx lr
2:	.word __init_array_start-1b
	.word __init_array_end-1b

.section .fini
	pop {r4,r5,r6,lr}
	tst lr,#1
	moveq pc,lr
	bx lr
