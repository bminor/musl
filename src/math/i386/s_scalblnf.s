.global ldexpf
.global scalbnf
.global scalblnf
ldexpf:
scalbnf:
scalblnf:
	fildl 8(%esp)
	flds 4(%esp)
	fscale
	fstp %st(1)
	ret
