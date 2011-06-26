.global remainderf
.type remainderf,@function
remainderf:
	flds 8(%esp)
	flds 4(%esp)
	jmp 1f
	
.global remainder
.type remainder,@function
remainder:
	fldl 12(%esp)
	fldl 4(%esp)
1:	fprem1
	fstsw %ax
	sahf
	jp 1b
	fstp %st(1)
	ret
