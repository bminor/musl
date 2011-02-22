.text
.global _start
_start:
	xorl %ebp,%ebp
	popl %ecx
	movl %esp,%eax
	andl $-16,%esp
	pushl %esp
	pushl %esp
	pushl %edx
	pushl %ebp
	pushl %ebp
	pushl %eax
	pushl %ecx
	call 1f
1:	addl $[main-.],(%esp)
	//pushl $main
	call __libc_start_main
1:	jmp 1b
