.text
.global __unmapself
.type   __unmapself,%function
__unmapself:
	call 1f
	.long -1
	.long -1
1:	popl %ecx
	xorl %ebx,%ebx
	xorl %edx,%edx
	movl $8,%esi
	movl $175,%eax
	int $128
	movl $91,%eax
	movl 4(%esp),%ebx
	movl 8(%esp),%ecx
	int $128
	xorl %ebx,%ebx
	movl $1,%eax
	int $128

.size __unmapself,.-__unmapself
