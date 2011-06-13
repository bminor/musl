2:	not %ecx
	sub $32,%esp
	fnstenv (%esp)
	and %ecx,4(%esp)
	or %edx,4(%esp)
	fldenv (%esp)
	add $32,%esp
	ret

.global feclearexcept
feclearexcept:	
	xor %eax,%eax
	mov 4(%esp),%ecx
	xor %edx,%edx
	test %ecx,%ecx
	jnz 2b
	ret

.global feraiseexcept
feraiseexcept:	
	xor %eax,%eax
	mov 4(%esp),%edx
	xor %ecx,%ecx
	test %edx,%edx
	jnz 2b
	ret

.global fesetround
fesetround:
	mov 4(%esp),%ecx
	xor %eax,%eax
	sub $32,%esp
	fnstenv (%esp)
	andb $0xf3,1(%esp)
	or %ecx,(%esp)
	fldenv (%esp)
	add $32,%esp
	ret

.global fegetround
fegetround:
	sub $28,%esp
	fnstenv (%esp)
	mov 4(%esp),%eax
	add $28,%esp
	and $0xc,%ah
	ret

.global fegetenv
fegetenv:
	mov 4(%esp),%ecx
	xor %eax,%eax
	fnstenv (%ecx)
	ret

.global fesetenv
fesetenv:
	mov 4(%esp),%ecx
	xor %eax,%eax
	test %ecx,%ecx
	jz 1f
	fldenv (%ecx)
	ret
1:	push %eax
	push %eax
	push %eax
	push %eax
	push %eax
	push %eax
	pushl $0x37f
	fldenv (%esp)
	add $28,%esp
	ret

.global fetestexcept
fetestexcept:
	mov 4(%esp),%ecx
	fnstsw %ax
	and %ecx,%eax
	ret
