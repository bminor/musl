.global atan
.type atan,@function
atan:
	fldl 4(%esp)
	fld1
	fpatan
	ret
