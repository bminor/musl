.global __restore_rt
.global __restore
__restore_rt:
__restore:
	movl $15, %eax
	syscall
