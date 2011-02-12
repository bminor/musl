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
	pushl $0
	pushl $0
	pushl %eax
	pushl %ecx
	pushl $main
	call __libc_start_main
.L0:	jmp .L0
