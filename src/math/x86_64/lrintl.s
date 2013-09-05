.global lrintl
.type lrintl,@function
lrintl:
	fldt 8(%rsp)
	fistpl 8(%rsp)
	movslq 8(%rsp),%rax
	ret
