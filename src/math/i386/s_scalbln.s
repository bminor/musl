.global ldexp
.global scalbn
.global scalbln
.type ldexp,@function
.type scalbn,@function
.type scalbln,@function
ldexp:
scalbn:
scalbln:
	fildl 12(%esp)
	fldl 4(%esp)
	fscale
	fstp %st(1)
	ret
