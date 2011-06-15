.text
.global __syscall_cp_asm
.type   __syscall_cp_asm,@function
__syscall_cp_asm:
	pushl %ebx
	pushl %esi
	pushl %edi
	pushl %ebp
	leal 20(%esp),%ebp
	call 1f
1:	popl %eax
	movl (%ebp),%ecx
	addl $[1f-1b],%eax
	movl %eax,4(%ecx)
	movl %esp,(%ecx)
	movl 8(%ecx),%eax
	testl %eax,%eax
	jnz 2f
	movl 4(%ebp),%eax
	movl 8(%ebp),%ebx
	movl 12(%ebp),%ecx
	movl 16(%ebp),%edx
	movl 20(%ebp),%esi
	movl 24(%ebp),%edi
	movl 28(%ebp),%ebp
1:	int $128
	popl %ebp
	popl %edi
	popl %esi
	popl %ebx
	xorl %edx,%edx
	movl 4(%esp),%ecx
	movl %edx,(%ecx)
	ret
2:	call __cancel
