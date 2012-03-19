.global log1p
.type log1p,@function
log1p:
	mov 8(%esp),%eax
	fldln2
	and $0x7fffffff,%eax
	fldl 4(%esp)
	cmp $0x3fd28f00,%eax
	ja 1f
	fyl2xp1
	ret
1:	fld1
	faddp
	fyl2x
	ret
