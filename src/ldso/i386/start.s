.text
.global _start
_start:
	xor %ebp,%ebp
	pop %edi
	mov %esp,%esi
	and $-16,%esp
	push %ebp
	push %ebp
	push %esi
	push %edi
	call __dynlink
	mov %esi,%esp
	push %edi
	xor %edx,%edx
	jmp *%eax
