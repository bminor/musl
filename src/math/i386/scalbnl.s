.global ldexpl
.type ldexpl,@function
ldexpl:
	nop

.global scalblnl
.type scalblnl,@function
scalblnl:
	nop

.global scalbnl
.type scalbnl,@function
scalbnl:
	fildl 16(%esp)
	fldt 4(%esp)
	fscale
	fstp %st(1)
	ret
