.text
.global _start
_start:
	mov (%rsp),%rdi
	lea 8(%rsp),%rsi
	lea _GLOBAL_OFFSET_TABLE_(%rip),%rdx
	call __dynlink
	xor %edx,%edx
	jmp *%rax
