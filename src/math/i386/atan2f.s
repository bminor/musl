.global atan2f
.type atan2f,@function
atan2f:
	flds 4(%esp)
	flds 8(%esp)
	fpatan
	ret
