.global modfl
.type modfl,@function
modfl:
	fldt 8(%rsp)
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
        fstpt (%rdi)
        ret

2:	fstpt (%rdi)
	mov 6(%rdi),%eax
	and $0x80000000,%eax
	mov %eax,8(%rsp)
	flds 8(%rsp)
	ret
