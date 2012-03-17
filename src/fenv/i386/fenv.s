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
	sub $4,%esp
	fnstcw (%esp)
	andb $0xf3,1(%esp)
	or %cx,(%esp)
	fldcw (%esp)
	add $4,%esp
	ret

.global fegetround
.type fegetround,@function
fegetround:
	sub $4,%esp
	fnstcw (%esp)
	mov (%esp),%ax
	add $4,%esp
	and $0xc00,%eax
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
