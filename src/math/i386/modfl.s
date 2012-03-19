.global modfl
.type modfl,@function
modfl:
	mov 16(%esp),%eax
	fldt 4(%esp)
	fld1
	fld %st(1)
1:	fprem
	fstsw %ax
	sahf
	jp 1b
	fstp %st(1)
	fsubr %st(1)
	fstpt (%eax)
	ret
