.global expm1l
.type expm1l,@function
expm1l:
	fldt 8(%rsp)
1:	fldl2e
	fmulp
	fld1
	fld %st(1)
	fabs
	fucom %st(1)
	fnstsw %ax
	fstp %st(0)
	fstp %st(0)
	sahf
	ja 1f
	f2xm1
	ret
1:	push %rax
	call 1f
	pop %rax
	fld1
	fsubrp
	ret

.global expl
.type expl,@function
expl:
	fldt 8(%rsp)
	fldl2e
	fmulp
	jmp 1f

.global exp2l
.type exp2l,@function
exp2l:
	fldt 8(%rsp)
1:	mov $0x467ff000,%eax
	mov %eax,-16(%rsp)
	mov $0x80000000,%eax
	mov %eax,-20(%rsp)
	xor %eax,%eax
	mov %eax,-24(%rsp)
	flds -16(%rsp)
	fld %st(1)
	fabs
	fucom %st(1)
	fnstsw
	fstp %st(0)
	fstp %st(0)
	sahf
	ja 2f
	fld %st(0)
	fistpl -16(%rsp)
	fildl -16(%rsp)
	fxch %st(1)
	fsub %st(1)
	mov $0x3fff,%eax
	add %eax,-16(%rsp)
	f2xm1
	fld1
	faddp
	fldt -24(%rsp)
	fmulp
	fstp %st(1)
	ret

2:	fld %st(0)
	fstpt -24(%rsp)
	mov -15(%rsp),%ah
	and $0x7f,%ah
	cmp $0x7f,%ah
	jne 1f
	decb -15(%rsp)
	fstp %st(0)
	fldt -24(%rsp)
1:	fld %st(0)
	frndint
	fxch %st(1)
	fsub %st(1)
	f2xm1
	fld1
	faddp
	fscale
	fstp %st(1)
	ret
