.global atan2
.type atan2,@function
atan2:
	fldl 4(%esp)
	fldl 12(%esp)
	fpatan
	ret
