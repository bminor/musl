.global sqrt
.type sqrt,@function
sqrt:	fldl 4(%esp)
	fsqrt
	ret
