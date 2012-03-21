.global asinl
.type asinl,@function
asinl:
	fldt 8(%rsp)
1:	fld %st(0)
	fmul %st(0)
	fld1
	fsubp %st(1)
	fsqrt
	fpatan
	ret

