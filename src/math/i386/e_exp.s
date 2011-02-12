.global expf
expf:
	mov 4(%esp),%eax
	flds 4(%esp)
	shr $23,%eax
	inc %al
	jz 1f
	jmp 0f

.global exp
exp:
	mov 8(%esp),%eax
	fldl 4(%esp)
	shl %eax
	cmp $0xffe00000,%eax
	jae 1f

0:	fldl2e
	fmulp
	fst %st(1)
	frndint
	fst %st(2)
	fsubrp
	f2xm1
	fld1
	faddp
	fscale
	fstp %st(1)
	ret

1:	fsts 4(%esp)
	cmpl $0xff800000,4(%esp)
	jnz 1f
	fstp %st(0)
	fldz
1:	ret
