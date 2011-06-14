.global __restore
__restore:
	popl %eax
	movl $119, %eax
	int $0x80

.global __restore_rt
__restore_rt:
	movl $173, %eax
	int $0x80
