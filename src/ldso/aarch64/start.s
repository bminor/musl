.global _dlstart
_dlstart:
	ldr x0,[sp]
	add x1,sp,#8
	bl __dynlink
	mov x1,sp
	ldr x2,[x1],#8
1:	sub x2,x2,1
	ldr x3,[x1],#8
	cmn x3,#1
	b.eq 1b
	add x2,x2,1
	str x3,[x1,#-8]!
	str x2,[x1,#-8]!
	mov sp,x1
	mov x1,x0
	mov x0,#0
	blr x1
