.weak _init
.weak _fini
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
	pushl $_fini
	pushl $_init
	pushl %eax
	pushl %ecx
	pushl $main
	call __libc_start_main
1:	jmp 1b
