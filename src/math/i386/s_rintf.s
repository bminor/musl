.global rintf
rintf:
	flds 4(%esp)
	frndint
	ret
