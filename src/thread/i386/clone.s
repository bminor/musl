.text
.global __uniclone
.type   __uniclone,%function
__uniclone:
	movl	4(%esp),%ecx
	subl	$24,%ecx
	movl	8(%esp),%eax
	movl	%eax,16(%ecx)
	movl	12(%esp),%eax
	movl	%eax,24(%ecx)
	pushl	%ebx
	pushl	%esi
	pushl	%edi
	pushl   %ebp
	movl    %eax,8(%eax)
	leal    20(%eax),%edx
	leal    4(%eax),%esi
	movl	%edx,%edi
	movl	$0x7d0f00,%ebx
	movl	$120,%eax
	int	$128
	popl    %ebp
	popl	%edi
	popl	%esi
	popl	%ebx
	ret
.size __uniclone,.-__uniclone
