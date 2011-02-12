.text
.global __clone
.type   __clone,%function
__clone:
	movl	8(%esp),%ecx
	andl	$0xfffffff0, %ecx
	subl	$28,%ecx
	movl	16(%esp),%eax
	movl	%eax,12(%ecx)
	movl	4(%esp),%eax
	movl	%eax,8(%ecx)
	pushl	%ebx
	pushl	%esi
	pushl	%edi
	movl	$120,%eax
	movl	12+12(%esp),%ebx
	movl	20+12(%esp),%edx
	movl	24+12(%esp),%esi
	movl	28+12(%esp),%edi
	int	$128
	popl	%edi
	popl	%esi
	popl	%ebx
	test	%eax,%eax
	jnz	1f
	xorl	%ebp,%ebp
	call	*%ebx
	movl	%eax, %ebx
	movl	$1, %eax
	int	$128
1:	
	movl %eax, 4(%esp)
	ret

.size __clone,.-__clone
