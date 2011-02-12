.global log10f
log10f:
	fldlg2
	flds 4(%esp)
	fyl2x
	ret
