.global memset
.type memset,@function
memset:
	movzbl %sil,%esi
	mov $0x101010101010101,%rax
	# 64-bit imul has 3-7 cycles latency, launch early
	imul %rsi,%rax

	cmp $16,%rdx
	jb 1f

	lea -1(%rdx),%rcx
	mov %rdi,%r8
	shr $3,%rcx
	mov %rax,-8(%rdi,%rdx)
	rep
	stosq
	mov %r8,%rax
	ret

1:	test %edx,%edx
	jz 1f

	mov %al,(%rdi)
	mov %al,-1(%rdi,%rdx)
	cmp $2,%edx
	jbe 1f

	mov %al,1(%rdi)
	mov %al,-2(%rdi,%rdx)
	cmp $4,%edx
	jbe 1f

	mov %eax,(%rdi)
	mov %eax,-4(%rdi,%rdx)
	cmp $8,%edx
	jbe 1f

	mov %eax,4(%rdi)
	mov %eax,-8(%rdi,%rdx)

1:	mov %rdi,%rax
	ret
