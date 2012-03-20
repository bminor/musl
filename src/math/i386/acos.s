.global acosf
.type acosf,@function
acosf:
	flds 4(%esp)
	jmp 1f

.global acosl
.type acosl,@function
acosl:
	fldt 4(%esp)
	jmp 1f

.global acos
.type acos,@function
acos:
	fldl 4(%esp)
1:	fld1
	fld %st(1)
	fld1
	fsubp
	fxch %st(2)
	faddp
	fdivp
	fsqrt
	fld1
	fxch %st(1)
	fpatan
	fld1
	fld1
	faddp
	fmulp
	ret
