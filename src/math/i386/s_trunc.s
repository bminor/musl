.global ceilf
ceilf:	flds 4(%esp)
	jmp 1f
	
.global ceil
ceil:	fldl 4(%esp)
1:	mov $0x08fb,%edx
	jmp 0f

.global floorf
floorf:	flds 4(%esp)
	jmp 1f

.global floor
floor:	fldl 4(%esp)
1:	mov $0x04f7,%edx
	jmp 0f

.global truncf
truncf:	flds 4(%esp)
	jmp 1f

.global trunc
trunc:	fldl 4(%esp)
1:	mov $0x0cff,%edx

0:	fstcw 4(%esp)
	mov 5(%esp),%ah
	or %dh,%ah
	and %dl,%ah
	xchg %ah,5(%esp)
	fldcw 4(%esp)
	frndint
	mov %ah,5(%esp)
	fldcw 4(%esp)
	ret
