2:	not %edi
	sub $32,%rsp
	fnstenv (%rsp)
	and %edi,4(%rsp)
	or %esi,4(%rsp)
	fldenv (%rsp)
	stmxcsr (%rsp)
	and %edi,(%rsp)
	ldmxcsr (%rsp)
	add $32,%rsp
	ret

.global feclearexcept
feclearexcept:	
	xor %eax,%eax
	xor %esi,%esi
	test %edi,%edi
	jnz 2b
	ret

.global feraiseexcept
feraiseexcept:	
	xor %eax,%eax
	mov %edi,%esi
	xor %edi,%edi
	test %esi,%esi
	jnz 2b
	ret

.global fesetround
fesetround:
	xor %eax,%eax
	sub $32,%rsp
	fnstenv (%rsp)
	andb $0xf3,1(%rsp)
	or %edi,(%rsp)
	fldenv (%rsp)
	stmxcsr (%rsp)
	shl $3,%edi
	andb $0x9f,1(%rsp)
	or %edi,(%rsp)
	ldmxcsr (%rsp)
	add $32,%rsp
	ret

.global fegetround
fegetround:
	push %rax
	stmxcsr (%rsp)
	pop %rax
	shr $3,%eax
	and $0xc,%ah
	ret

.global fegetenv
fegetenv:
	xor %eax,%eax
	fnstenv (%rdi)
	stmxcsr 28(%rdi)
	ret

.global fesetenv
fesetenv:
	xor %eax,%eax
	inc %rdi
	jz 1f
	fldenv -1(%rdi)
	ldmxcsr 27(%rdi)
	ret
1:	push %rax
	push %rax
	push %rax
	pushq $0x37f
	fldenv (%rsp)
	pushq $0x1f80
	ldmxcsr (%rsp)
	add $40,%rsp
	ret

.global fetestexcept
fetestexcept:
	push %rax
	stmxcsr (%rsp)
	pop %rsi
	fnstsw %ax
	or %esi,%eax
	and %edi,%eax
	ret
