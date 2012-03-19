.global modff
.type modff,@function
modff:
	mov 8(%esp),%eax
	flds 4(%esp)
	fld1
	fld %st(1)
1:	fprem
	fstsw %ax
	sahf
	jp 1b
	fstp %st(1)
	fsubr %st(1)
	fstps (%eax)
	ret
