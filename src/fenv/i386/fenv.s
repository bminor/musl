.global feclearexcept
.type feclearexcept,@function
feclearexcept:	
	mov 4(%esp),%ecx
	not %ecx
	test $0x3f,%ecx
	jnz 2f
1:	fnclex
	xor %eax,%eax
	ret
2:	fnstsw %ax
	and %ecx,%eax
	jz 1b
	sub $32,%esp
	fnstenv (%esp)
	mov %al,4(%esp)
	fldenv (%esp)
	add $32,%esp
	xor %eax,%eax
	ret

.global feraiseexcept
.type feraiseexcept,@function
feraiseexcept:	
	mov 4(%esp),%eax
	sub $32,%esp
	fnstenv (%esp)
	or %al,4(%esp)
	fldenv (%esp)
	add $32,%esp
	xor %eax,%eax
	ret

.global fesetround
.type fesetround,@function
fesetround:
	mov 4(%esp),%ecx
	push %eax
	xor %eax,%eax
	fnstcw (%esp)
	andb $0xf3,1(%esp)
	or %ch,1(%esp)
	fldcw (%esp)
	pop %ecx
	ret

.global fegetround
.type fegetround,@function
fegetround:
	push %eax
	fnstcw (%esp)
	pop %eax
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
