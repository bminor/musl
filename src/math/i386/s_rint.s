.global rint
rint:
	fldl 4(%esp)
	frndint
	ret
