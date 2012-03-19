.global modf
.type modf,@function
modf:
	mov 12(%esp),%eax
	fldl 4(%esp)
	fld1
	fld %st(1)
1:	fprem
	fstsw %ax
	sahf
	jp 1b
	fstp %st(1)
	fsubr %st(1)
	fstpl (%eax)
	ret
