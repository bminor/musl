.text
.global __pthread_register_cancel
.type   __pthread_register_cancel,@function
__pthread_register_cancel:
	pushl %eax
	call __pthread_do_register
	popl %eax
	ret

.global __pthread_unregister_cancel
.type   __pthread_unregister_cancel,@function
__pthread_unregister_cancel:
	pushl %eax
	call __pthread_do_unregister
	popl %eax
	ret

.global __pthread_unwind_next
.type   __pthread_unwind_next,@function
__pthread_unwind_next:
	pushl %eax
	call __pthread_do_unwind
	popl %eax
__pthread_do_unwind:
__pthread_do_register:
__pthread_do_unregister:
	ret

.weak __pthread_do_unwind
.weak __pthread_do_register
.weak __pthread_do_unregister
.type __pthread_do_unwind,@function
.type __pthread_do_register,@function
.type __pthread_do_unregister,@function
