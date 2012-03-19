.global llrintl
.type llrintl,@function
llrintl:
	fldt 4(%esp)
	fistpq 4(%esp)
	mov 4(%esp),%eax
	mov 8(%esp),%edx
	ret
