.global sigsetjmp
.type sigsetjmp,@function
sigsetjmp:
	mov 4(%esp),%eax
	mov 8(%esp),%ecx
	mov %ecx,24(%eax)
	jecxz 1f
	add $28,%eax
	push %eax
	push $0
	push $2
	call sigprocmask
	add $12,%esp
1:	jmp setjmp
