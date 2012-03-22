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
	fsqrt
	fxch %st(2)
	faddp
	fsqrt
	fpatan
	fld1
	fld1
	faddp
	fmulp
	ret
