.global _dlstart
_dlstart:
	l.jal	1f
	 l.nop
1:	l.movhi	r5, gotpchi(_GLOBAL_OFFSET_TABLE_+0)
	l.ori	r5, r5, gotpclo(_GLOBAL_OFFSET_TABLE_+4)
	l.add	r5, r5, r9
	l.movhi	r3, gotoffhi(_DYNAMIC)
	l.ori	r3, r3, gotofflo(_DYNAMIC)
	l.add	r5, r5, r3

	l.lwz	r3, 0(r1)
	l.addi	r4, r1, 4
	l.jal	plt(__reloc_self)
	 l.addi	r1, r1, -16

	l.lwz	r3, 16(r1)
	l.jal	plt(__dynlink)
	 l.addi	r4, r1, 20
	l.addi	r1, r1, 16

	l.lwz	r4, 0(r1)
1:	l.addi	r4, r4, -1
	l.lwz	r5, 4(r1)
	l.sfeqi	r5, -1
	l.bf	1b
	 l.addi	r1, r1, 4

	l.addi	r4, r4, 1
	l.addi	r1, r1, -4
	l.sw	0(r1), r4

	l.jr	r11
	 l.ori	r3, r0, 0
