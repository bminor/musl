.global expm1f
.type expm1f,@function
expm1f:
	flds 4(%esp)
	jmp 1f

.global expm1l
.type expm1l,@function
expm1l:
	fldt 4(%esp)
	jmp 1f

.global expm1
.type expm1,@function
expm1:
	fldl 4(%esp)
1:	fldl2e
	fmulp
	fld %st(0)
	frndint
	fldz
	fcompi
	jnz 1f
	fstp %st(0)
	f2xm1
	ret
1:	fxch %st(1)
	fsub %st(1)
	f2xm1
	fld1
	faddp
	fscale
	fld1
	fsubrp
	fstp %st(1)
	ret
