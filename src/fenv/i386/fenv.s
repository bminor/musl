2:	not %ecx
	sub $32,%esp
	fnstenv (%esp)
	and %ecx,4(%esp)
	or %edx,4(%esp)
	fldenv (%esp)
	add $32,%esp
	ret

.global feclearexcept
.type feclearexcept,@function
feclearexcept:	
	xor %eax,%eax
	mov 4(%esp),%ecx
	xor %edx,%edx
	test %ecx,%ecx
	jnz 2b
	ret

.global feraiseexcept
.type feraiseexcept,@function
feraiseexcept:	
	xor %eax,%eax
	mov 4(%esp),%edx
	xor %ecx,%ecx
	test %edx,%edx
	jnz 2b
	ret

.global fesetround
.type fesetround,@function
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
.type fegetround,@function
fegetround:
	sub $28,%esp
	fnstenv (%esp)
	mov 4(%esp),%eax
	add $28,%esp
	and $0xc,%ah
	ret

.global fegetenv
.type fegetenv,@function
fegetenv:
	mov 4(%esp),%ecx
	xor %eax,%eax
	fnstenv (%ecx)
	ret

.global fesetenv
.type fesetenv,@function
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
.type fetestexcept,@function
fetestexcept:
	mov 4(%esp),%ecx
	fnstsw %ax
	and %ecx,%eax
	ret
