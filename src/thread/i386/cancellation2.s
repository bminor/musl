.text
.global __pthread_register_cancel
__pthread_register_cancel:
	pushl %eax
	call __pthread_register_cancel_3
	popl %eax
	ret

.global __pthread_unregister_cancel
__pthread_unregister_cancel:
	pushl %eax
	call __pthread_unregister_cancel_3
	popl %eax
	ret
