.text
.global _start
_start:
	ldr r0,[sp]
	add r1,sp,#4
	bl __dynlink
	mov r1,r0
	mov r0,#0
	tst r1,#1
	moveq pc,r1
	bx r1
