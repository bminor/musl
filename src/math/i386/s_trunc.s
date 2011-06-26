.global ceilf
.type ceilf,@function
ceilf:	flds 4(%esp)
	jmp 1f
	
.global ceil
.type ceil,@function
ceil:	fldl 4(%esp)
1:	mov $0x08fb,%edx
	jmp 0f

.global floorf
.type floorf,@function
floorf:	flds 4(%esp)
	jmp 1f

.global floor
.type floor,@function
floor:	fldl 4(%esp)
1:	mov $0x04f7,%edx
	jmp 0f

.global truncf
.type truncf,@function
truncf:	flds 4(%esp)
	jmp 1f

.global trunc
.type trunc,@function
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
