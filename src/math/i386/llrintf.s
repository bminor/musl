.global llrintf
.type llrintf,@function
llrintf:
	sub $8,%esp
	flds 12(%esp)
	fistpq (%esp)
	pop %eax
	pop %edx
	ret
