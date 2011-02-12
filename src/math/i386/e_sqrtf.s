.global sqrtf
sqrtf:	flds 4(%esp)
	fsqrt
	ret
