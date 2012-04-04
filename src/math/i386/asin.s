.global asinf
.type asinf,@function
asinf:
	flds 4(%esp)
	jmp 1f

.global asinl
.type asinl,@function
asinl:
	fldt 4(%esp)
	jmp 1f

.global asin
.type asin,@function
asin:
	fldl 4(%esp)
1:	fld %st(0)
	fld1
	fsub %st(0),%st(1)
	fadd %st(2)
	fmulp
	fsqrt
	fpatan
	ret
