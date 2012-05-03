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
1:	addl $_GLOBAL_OFFSET_TABLE_,(%esp)
	pop %ebx
	call 1f
1:	addl $[_fini-.],(%esp)
	call 1f
1:	addl $[_init-.],(%esp)
	pushl %eax
	pushl %ecx
	pushl main@GOT(%ebx)
	call __libc_start_main@plt
1:	jmp 1b
