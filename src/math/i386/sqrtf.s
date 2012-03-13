.global sqrtf
.type sqrtf,@function
sqrtf:	flds 4(%esp)
	fsqrt
	ret
