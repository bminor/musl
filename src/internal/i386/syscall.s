.global __syscall
.type __syscall,@function
__syscall:
	pushl %ebx
	pushl %esi
	pushl %edi
	pushl %ebp
	movl 20(%esp),%eax
	movl 24(%esp),%ebx
	movl 28(%esp),%ecx
	movl 32(%esp),%edx
	movl 36(%esp),%esi
	movl 40(%esp),%edi
	movl 44(%esp),%ebp
	int $128
	popl %ebp
	popl %edi
	popl %esi
	popl %ebx
	ret
