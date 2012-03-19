.global remquof
.type remquof,@function
remquof:
	mov 12(%esp),%ecx
	fldl 4(%esp)
	fldl 8(%esp)
	jmp 1f

.global remquol
.type remquol,@function
remquol:
	mov 28(%esp),%ecx
	fldl 4(%esp)
	fldl 16(%esp)
	jmp 1f

.global remquo
.type remquo,@function
remquo:
	mov 20(%esp),%ecx
	fldl 4(%esp)
	fldl 12(%esp)
1:	fld %st(1)
1:      fprem1
	fnstsw %ax
	sahf
	jp 1b
	fsubr %st(0),%st(2)
	fxch %st(2)
	fdivp
	mov $0x4f000000,%eax
	mov %eax,4(%esp)
	flds 4(%esp)
	fxch %st(1)
1:	fprem
	fnstsw %ax
	sahf
	jp 1b
	fistpl (%ecx)
	fstp %st(0)
	ret
