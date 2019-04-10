.global __syscall
.hidden __syscall
.type __syscall,@function
__syscall:
	push %ebx
	push %esi
	push %edi
	push %ebp
	mov 20(%esp),%eax
	mov 24(%esp),%ebx
	mov 28(%esp),%ecx
	mov 32(%esp),%edx
	mov 36(%esp),%esi
	mov 40(%esp),%edi
	mov 44(%esp),%ebp
	call *%gs:16
	pop %ebp
	pop %edi
	pop %esi
	pop %ebx
	ret
