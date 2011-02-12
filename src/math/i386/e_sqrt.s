.global sqrt
sqrt:	fldl 4(%esp)
	fsqrt
	ret
