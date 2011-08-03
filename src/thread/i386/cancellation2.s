.text
.global __pthread_register_cancel
.type   __pthread_register_cancel,@function
__pthread_register_cancel:
	pushl %eax
	call __pthread_register_cancel_3
	popl %eax
	ret

.global __pthread_unregister_cancel
.type   __pthread_unregister_cancel,@function
__pthread_unregister_cancel:
	pushl %eax
	call __pthread_unregister_cancel_3
	popl %eax
	ret

.global __pthread_unwind_next
.type   __pthread_unwind_next,@function
__pthread_unwind_next:
	pushl %eax
	call __pthread_unwind_next_3
	popl %eax
	ret
