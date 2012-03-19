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
1:	fld %st(0)
	fmul %st(0)
	fld1
	fsubp %st(1)
	fsqrt
	fxch %st(1)
	fpatan
	ret
