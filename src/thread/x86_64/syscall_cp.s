.text
.global __syscall_cp_asm
.type   __syscall_cp_asm,@function
__syscall_cp_asm:
	lea 1f(%rip),%rax
	mov %rax,8(%rdi)
	mov %rsp,(%rdi)
	mov 16(%rdi),%eax
	test %eax,%eax
	jnz 2f
	mov %rdi,%r11
	mov %rsi,%rax
	mov %rdx,%rdi
	mov %rcx,%rsi
	mov %r8,%rdx
	mov %r9,%r10
	mov 8(%rsp),%r8
	mov 16(%rsp),%r9
	mov %r11,8(%rsp)
1:	syscall
	xor %ecx,%ecx
	mov 8(%rsp),%rdi
	mov %rcx,(%rdi)
	ret
2:	call __cancel
