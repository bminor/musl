.global modff
.type modff,@function
modff:
	mov 8(%esp),%ecx
	flds 4(%esp)
	fxam
	fnstsw %ax
	sahf
        jnp 1f
        jc 2f
1:      fld1
        fld %st(1)
1:      fprem
        fnstsw %ax
        sahf
        jp 1b
        fstp %st(1)
        fsubr %st(0),%st(1)
        fxch %st(1)
        fstps (%ecx)
        ret

2:	fstps (%ecx)
	mov (%ecx),%eax
	and $0x80000000,%eax
	mov %eax,4(%esp)
	flds 4(%esp)
	ret
