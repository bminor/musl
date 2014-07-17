.global dlsym
.type   dlsym,@function
dlsym:
	l.j	plt(__dlsym)
	 l.ori	r5, r9, 0
