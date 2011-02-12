.global log10
log10:
	fldlg2
	fldl 4(%esp)
	fyl2x
	ret
