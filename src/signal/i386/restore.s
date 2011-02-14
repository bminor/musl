.global __restore
.type __restore,%function
__restore:
	popl %eax
	movl $119, %eax
	int $0x80
.size __restore,.-__restore

.global __restore_rt
.type __restore_rt,%function
__restore_rt:
	movl $173, %eax
	int $0x80
.size __restore_rt,.-__restore_rt

