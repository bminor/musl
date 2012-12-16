# exp(x) = 2^hi + 2^hi (2^lo - 1)
# where hi+lo = log2e*x with 128bit precision
# exact log2e*x calculation depends on nearest rounding mode
# using the exact multiplication method of Dekker and Veltkamp

.global expl
.type expl,@function
expl:
	fldt 8(%rsp)

		# special cases: 2*x is +-inf, nan or |x| < 0x1p-32
		# check (exponent|0x8000)+2 < 0xbfff+2-32
	movw 16(%rsp), %ax
	movw %ax, %dx
	orw $0x8000, %dx
	addw $2, %dx
	cmpw $0xbfff-30, %dx
	jnb 3f
	cmpw $1, %dx
	jbe 1f
		# if |x|<0x1p-32 return 1+x
	fld1
	jmp 2f
1:	testw %ax, %ax
	jns 1f
		# if 2*x == -inf,-nan return -0/x
	fldz
	fchs
	fdivp
	ret
		# if 2*x == inf,nan return 2*x
1:	fld %st(0)
2:	faddp
	ret

		# should be 0x1.71547652b82fe178p0 == 0x3fff b8aa3b29 5c17f0bc
		# it will be wrong on non-nearest rounding mode
3:	fldl2e
	subq $48, %rsp
		# hi = log2e_hi*x
		# 2^hi = exp2l(hi)
	fmul %st(1),%st
	fld %st(0)
	fstpt (%rsp)
	fstpt 16(%rsp)
	fstpt 32(%rsp)
	call exp2l
		# if 2^hi == inf return 2^hi
	fld %st(0)
	fstpt (%rsp)
	cmpw $0x7fff, 8(%rsp)
	je 1f
	fldt 32(%rsp)
	fldt 16(%rsp)
		# fpu stack: 2^hi x hi
		# exact mult: x*log2e
	fld %st(1)
		# c = 0x1p32+1
	movq $0x41f0000000100000,%rax
	pushq %rax
	fldl (%rsp)
		# xh = x - c*x + c*x
		# xl = x - xh
	fmulp
	fld %st(2)
	fsub %st(1), %st
	faddp
	fld %st(2)
	fsub %st(1), %st
		# yh = log2e_hi - c*log2e_hi + c*log2e_hi
	movq $0x3ff7154765200000,%rax
	pushq %rax
	fldl (%rsp)
		# fpu stack: 2^hi x hi xh xl yh
		# lo = hi - xh*yh + xl*yh
	fld %st(2)
	fmul %st(1), %st
	fsubp %st, %st(4)
	fmul %st(1), %st
	faddp %st, %st(3)
		# yl = log2e_hi - yh
	movq $0x3de705fc2f000000,%rax
	pushq %rax
	fldl (%rsp)
		# fpu stack: 2^hi x lo xh xl yl
		# lo += xh*yl + xl*yl
	fmul %st, %st(2)
	fmulp %st, %st(1)
	fxch %st(2)
	faddp
	faddp
		# log2e_lo
	movq $0xbfbe,%rax
	pushq %rax
	movq $0x82f0025f2dc582ee,%rax
	pushq %rax
	fldt (%rsp)
	addq $40,%rsp
		# fpu stack: 2^hi x lo log2e_lo
		# lo += log2e_lo*x
		# return 2^hi + 2^hi (2^lo - 1)
	fmulp %st, %st(2)
	faddp
	f2xm1
	fmul %st(1), %st
	faddp
1:	addq $48, %rsp
	ret
