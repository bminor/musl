.text
.global _start
_start:
	xorl %ebp,%ebp
	popl %ecx
	movl %esp,%eax
	andl $-16,%esp
	movl %eax,%esi
	movl %ecx,%edi
	pushl %ebp
	call 1f
1:	addl $_GLOBAL_OFFSET_TABLE_,(%esp)
	pushl %eax
	pushl %ecx
	call __dynlink
	movl %esi,%esp
	pushl %edi
	xorl %edx,%edx
	jmp *%eax
