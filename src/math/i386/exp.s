.global expm1f
.type expm1f,@function
expm1f:
	flds 4(%esp)
	jmp 1f

.global expm1l
.type expm1l,@function
expm1l:
	fldt 4(%esp)
	jmp 1f

.global expm1
.type expm1,@function
expm1:
	fldl 4(%esp)
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
1:	call 1f
	fld1
	fsubrp
	ret

.global exp2f
.type exp2f,@function
exp2f:
	flds 4(%esp)
	jmp 1f

.global exp2l
.type exp2l,@function
exp2l:
	fldt 4(%esp)
	jmp 1f

.global expf
.type expf,@function
expf:
	flds 4(%esp)
	jmp 2f

.global expl
.type expl,@function
expl:
	fldt 4(%esp)
	jmp 2f

.global exp
.type exp,@function
exp:
	fldl 4(%esp)
2:	fldl2e
	fmulp
	jmp 1f

.global exp2
.type exp2,@function
exp2:
	fldl 4(%esp)
1:	pushl $0x467ff000
	flds (%esp)
	xorl %eax,%eax
	pushl $0x80000000
	push %eax
	fld %st(1)
	fabs
	fucom %st(1)
	fnstsw
	fstp %st(0)
	fstp %st(0)
	sahf
	ja 2f
	fld %st(0)
	fistpl 8(%esp)
	fildl 8(%esp)
	fxch %st(1)
	fsub %st(1)
	mov $0x3fff,%eax
	add %eax,8(%esp)
	f2xm1
	fld1
	faddp
	fldt (%esp)
	fmulp
	fstp %st(1)
	add $12,%esp
	ret

2:	fld %st(0)
	fstpt (%esp)
	mov 9(%esp),%ah
	and $0x7f,%ah
	cmp $0x7f,%ah
	jne 1f
	decb 9(%esp)
	fstp %st(0)
	fldt (%esp)
1:	fld %st(0)
	frndint
	fxch %st(1)
	fsub %st(1)
	f2xm1
	fld1
	faddp
	fscale
	fstp %st(1)
	add $12,%esp
	ret
