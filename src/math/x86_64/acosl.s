.global acosl
.type acosl,@function
acosl:
	fldt 8(%rsp)
	fld1
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
