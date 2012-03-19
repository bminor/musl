.global atanf
.type atanf,@function
atanf:
	flds 4(%esp)
	fld1
	fpatan
	ret
