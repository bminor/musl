.text
.global __set_thread_area
.type   __set_thread_area,%function
__set_thread_area:
	pushl %ebx
	movl 8(%esp),%ecx
	movl $-1,4(%ecx)
	movl %ecx,8(%ecx)
	movl $0xfffff,12(%ecx)
	movl $0x51,16(%ecx)
	leal 4(%ecx),%ebx
	movl $243,%eax
	int $128
	popl %ebx
	testl %eax,%eax
	jnz 1f
	movl 4(%ecx),%ecx
	leal 3(,%ecx,8),%ecx
	movw %cx,%gs
1:
	ret
.size __set_thread_area,.-__set_thread_area
