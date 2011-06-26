.global ldexpf
.global scalbnf
.global scalblnf
.type ldexpf,@function
.type scalbnf,@function
.type scalblnf,@function
ldexpf:
scalbnf:
scalblnf:
	fildl 8(%esp)
	flds 4(%esp)
	fscale
	fstp %st(1)
	ret
