.global acosl
.type acosl,@function
acosl:
	fldt 8(%rsp)
	fld1
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
