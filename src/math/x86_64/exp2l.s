.global expm1l
.type expm1l,@function
expm1l:
	fldt 8(%rsp)
	fldl2e
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
	flds -16(%rsp)    # 16380
	fld %st(1)
	fabs
	fucom %st(1)
	fnstsw
	fstp %st(0)
	fstp %st(0)
	sahf
	ja 3f             # |x| > 16380
	jp 2f             # x is nan (avoid invalid except in fistp)
	fld %st(0)
	fistpl -16(%rsp)
	fildl -16(%rsp)
	fxch %st(1)
	fsub %st(1)
	mov $0x3fff,%eax
	add %eax,-16(%rsp)
	f2xm1
	fld1
	faddp             # 2^(x-rint(x))
	fldt -24(%rsp)    # 2^rint(x)
	fmulp
2:	fstp %st(1)
	ret

3:	fld %st(0)
	fstpt -24(%rsp)
	fld1
	mov -15(%rsp),%ax
	and $0x7fff,%ax
	cmp $0x7fff,%ax
	je 1f             # x = +-inf
	fld %st(1)
	frndint
	fxch %st(2)
	fsub %st(2)       # st(0)=x-rint(x), st(1)=1, st(2)=rint(x)
	f2xm1
	faddp             # 2^(x-rint(x))
1:	fscale
	fstp %st(1)
	ret
