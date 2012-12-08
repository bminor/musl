.section .init
.global _init
_init:
	push {r0,r1,r2,r4,r5,lr}

.weak __fini_array_start
.weak __fini_array_end

.section .fini
.global _fini
_fini:
	push {r4,r5,r6,lr}
	adr lr, 1f
	ldr r4, 2f
	ldr r5, 2f+4
	add r4, r4, lr
	add r5, r5, lr
1:	adr lr, 1b
	cmp r4, r5
	beq 3f
	ldmia r4!, {r3}
	tst r3,#1
	moveq pc,r3
	bx r3
2:	.word __fini_array_start-1b
	.word __fini_array_end-1b
3:	
