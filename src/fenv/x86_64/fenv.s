.global feclearexcept
.type feclearexcept,@function
feclearexcept:
	mov %edi,%ecx
	not %ecx
	stmxcsr -8(%rsp)
	and %ecx,-8(%rsp)
	ldmxcsr -8(%rsp)
	test $0x3f,%ecx
	jnz 2f
1:	fnclex
	xor %eax,%eax
	ret
2:	fnstsw %ax
	and %ecx,%eax
	jz 1b
	sub $32,%rsp
	fnstenv (%rsp)
	mov %al,4(%rsp)
	fldenv (%rsp)
	add $32,%rsp
	xor %eax,%eax
	ret

.global feraiseexcept
.type feraiseexcept,@function
feraiseexcept:	
	stmxcsr -8(%rsp)
	or %edi,-8(%rsp)
	ldmxcsr -8(%rsp)
	fnstenv -32(%rsp)
	or %edi,-28(%rsp)
	fldenv -32(%rsp)
	xor %eax,%eax
	ret

.global fesetround
.type fesetround,@function
fesetround:
	push %rax
	xor %eax,%eax
	mov %edi,%ecx
	fnstcw (%rsp)
	andb $0xf3,1(%rsp)
	or %ch,1(%rsp)
	fldcw (%rsp)
	stmxcsr (%rsp)
	shl $3,%ch
	andb $0x9f,1(%rsp)
	or %ch,1(%rsp)
	ldmxcsr (%rsp)
	pop %rcx
	ret

.global fegetround
.type fegetround,@function
fegetround:
	push %rax
	stmxcsr (%rsp)
	pop %rax
	shr $3,%eax
	and $0xc00,%eax
	ret

.global fegetenv
.type fegetenv,@function
fegetenv:
	xor %eax,%eax
	fnstenv (%rdi)
	stmxcsr 28(%rdi)
	ret

.global fesetenv
.type fesetenv,@function
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
.type fetestexcept,@function
fetestexcept:
	push %rax
	stmxcsr (%rsp)
	pop %rsi
	fnstsw %ax
	or %esi,%eax
	and %edi,%eax
	ret
