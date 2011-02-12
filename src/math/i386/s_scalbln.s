.global ldexp
.global scalbn
.global scalbln
ldexp:
scalbn:
scalbln:
	fildl 12(%esp)
	fldl 4(%esp)
	fscale
	fstp %st(1)
	ret
