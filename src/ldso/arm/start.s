.text
.global _start
_start:
	ldr r0,[sp]
	add r1,sp,#4
	ldr r2,2f
	add r2,pc,r2
1:	bl __dynlink
	mov r1,r0
	mov r0,#0
	tst r1,#1
	moveq pc,r1
	bx r1
2:	.word _GLOBAL_OFFSET_TABLE_ - 1b - 4
