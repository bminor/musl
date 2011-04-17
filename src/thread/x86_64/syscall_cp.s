.text
.global __syscall_cp_asm
.type   __syscall_cp_asm,%function
__syscall_cp_asm:
	lea 1f(%rip),%rax
	mov %rax,8(%rdi)
	mov %rsp,(%rdi)
	mov 16(%rdi),%eax
	test %eax,%eax
	jnz 2f
	mov %rsi,%rax
	mov %rdx,%rdi
	mov %rcx,%rsi
	mov %r8,%rdx
	mov %r9,%r10
	mov 8(%rsp),%r8
	mov 16(%rsp),%r9
1:	syscall
	ret
2:	xor %edi,%edi
	mov %rdi,8(%r10)
	mov %rdi,(%r10)
	dec %rdi
	jmp pthread_exit
