.global __setjmp
.global _setjmp
.global setjmp
.type __setjmp,%function
.type _setjmp,%function
.type setjmp,%function
__setjmp:
_setjmp:
setjmp:
	movl 4(%esp), %eax
	movl    %ebx, (%eax)
	movl    %esi, 4(%eax)
	movl    %edi, 8(%eax)
	movl    %ebp, 12(%eax)
	leal 4(%esp), %ecx
	movl    %ecx, 16(%eax)
	movl  (%esp), %ecx
	movl    %ecx, 20(%eax)
	xorl    %eax, %eax
	ret
.size __setjmp,.-__setjmp
.size _setjmp,.-_setjmp
.size setjmp,.-setjmp
