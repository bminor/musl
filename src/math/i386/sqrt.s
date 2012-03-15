.global sqrt
.type sqrt,@function
sqrt:	fldl 4(%esp)
	fsqrt
	fstsw %ax
	sub $12,%esp
	fld %st(0)
	fstpt (%esp)
	mov (%esp),%ecx
	and $0x7ff,%ecx
	cmp $0x400,%ecx
	jnz 1f
	mov 8(%esp),%ecx
	or $0xffff8000,%ecx
	inc %ecx
	jz 1f
	and $0x200,%eax
	sub $0x100,%eax
	sub %eax,(%esp)
	fstp %st(0)
	fldt (%esp)
1:	add $12,%esp
	fstpl 4(%esp)
	fldl 4(%esp)
	ret
