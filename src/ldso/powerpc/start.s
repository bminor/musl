	.global _start
	.type   _start,@function
_start:
	bl      1f
2:	.long   _DYNAMIC-2b
1:	mflr    5
	lwz     0, 0(5)
	add     5, 0, 5
	lwz     3, 0(1)
	addi    4, 1, 4
	addi    1, 1, -16
	bl      __reloc_self

	lwz     3, 16(1)
	addi    4, 1, 20
	bl      __dynlink
	addi    1, 1, 16

	mtlr    3
	li      3, 0
	blr
