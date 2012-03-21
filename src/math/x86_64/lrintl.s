.global lrintl
.type lrintl,@function
lrintl:
	fldt 8(%rsp)
	fistpl 8(%rsp)
	mov 8(%rsp),%eax
	ret
