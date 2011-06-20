.text
.global _start
_start:
	xor %ebp,%ebp
	pop %ecx
	mov %esp,%eax
	and $-16,%esp
	mov %eax,%esi
	mov %ecx,%edi
	push %ebp
	call 1f
1:	addl $_GLOBAL_OFFSET_TABLE_,(%esp)
	push %eax
	push %ecx
	call __dynlink
	mov %esi,%esp
	push %edi
	xor %edx,%edx
	jmp *%eax
