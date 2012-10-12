.hidden __sysinfo

.global __vsyscall
.type __vsyscall,@function
__vsyscall:
	push %edi
	push %ebx
	mov %edx,%ebx
	mov %edi,%edx
	mov 12(%esp),%edi
	push %eax
	call 1f
2:	pop %ebx
	pop %ebx
	pop %edi
	ret

1:	mov (%esp),%eax
	add $[__sysinfo-2b],%eax
	mov (%eax),%eax
	test %eax,%eax
	jz 1f
	push %eax
	mov 8(%esp),%eax
	ret
1:	mov 4(%esp),%eax
	int $128
	ret

.global __vsyscall6
.type __vsyscall6,@function
__vsyscall6:
	push %ebp
	push %eax
	mov 12(%esp), %ebp
	mov (%ebp), %eax
	mov 4(%ebp), %ebp
	push %eax
	mov 4(%esp),%eax
	call __vsyscall
	pop %ebp
	pop %ebp
	pop %ebp
	ret

.global __syscall
.type __syscall,@function
__syscall:
	lea 24(%esp),%eax
	push %esi
	push %edi
	push %eax
	mov 16(%esp),%eax
	mov 20(%esp),%edx
	mov 24(%esp),%ecx
	mov 28(%esp),%edi
	mov 32(%esp),%esi
	call __vsyscall6
	pop %edi
	pop %edi
	pop %esi
	ret
