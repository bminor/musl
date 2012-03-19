.global log1pf
.type log1pf,@function
log1pf:
	mov 4(%esp),%eax
	fldln2
	and $0x7fffffff,%eax
	flds 4(%esp)
	cmp $0x3e940000,%eax
	ja 1f
	fyl2xp1
	ret
1:	fld1
	faddp
	fyl2x
	ret
