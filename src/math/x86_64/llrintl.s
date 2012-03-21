.global llrintl
.type llrintl,@function
llrintl:
	fldt 8(%rsp)
	fistpq 8(%rsp)
	mov 8(%rsp),%rax
	ret
