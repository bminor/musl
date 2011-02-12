.global fabs
fabs:
	fldl 4(%esp)
	fabs
	ret
