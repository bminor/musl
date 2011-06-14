.text
.global __pthread_unwind_next
__pthread_unwind_next:
	pushl %eax
	call __pthread_unwind_next_3
	popl %eax
	ret
