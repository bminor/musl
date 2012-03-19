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
1:	fxam
	fnstsw %ax
	sahf
	jnp 1f
	jnc 1f
	fstps 4(%esp)
	mov $0xfe,%al
	and %al,7(%esp)
	flds 4(%esp)
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
