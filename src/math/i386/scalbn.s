.global ldexp
.type ldexp,@function
ldexp:
	nop

.global scalbln
.type scalbln,@function
scalbln:
	nop

.global scalbn
.type scalbn,@function
scalbn:
	fildl 12(%esp)
	fldl 4(%esp)
	fscale
	fstp %st(1)
	fstpl 4(%esp)
	fldl 4(%esp)
	ret
