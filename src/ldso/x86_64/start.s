.text
.global _start
_start:
	mov (%rsp),%rdi
	lea 8(%rsp),%rsi
	call __dynlink
	xor %edx,%edx
	jmp *%rax
