.global ldexpf
.type ldexpf,@function
ldexpf:
	nop

.global scalblnf
.type scalblnf,@function
scalblnf:
	nop

.global scalbnf
.type scalbnf,@function
scalbnf:
	fildl 8(%esp)
	flds 4(%esp)
	fscale
	fstp %st(1)
	fstps 4(%esp)
	flds 4(%esp)
	ret
