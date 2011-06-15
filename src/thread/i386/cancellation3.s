.text
.global __pthread_unwind_next
.type   __pthread_unwind_next,@function
__pthread_unwind_next:
	pushl %eax
	call __pthread_unwind_next_3
	popl %eax
	ret
