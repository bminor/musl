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
	call 1f
1:	addl $[_fini-.],(%esp)
	call 1f
1:	addl $[_init-.],(%esp)
	pushl %eax
	pushl %ecx
	call 1f
1:	addl $[main-.],(%esp)
	call __libc_start_main
1:	jmp 1b
