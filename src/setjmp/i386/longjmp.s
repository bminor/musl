.global _longjmp
.global longjmp
.type _longjmp,%function
.type longjmp,%function
_longjmp:
longjmp:
	movl  4(%esp),%edx
	movl  8(%esp),%eax
	testl    %eax,%eax
	jnz .L0
	incl     %eax
.L0:
	movl   (%edx),%ebx
	movl  4(%edx),%esi
	movl  8(%edx),%edi
	movl 12(%edx),%ebp
	movl 16(%edx),%ecx
	movl     %ecx,%esp
	movl 20(%edx),%ecx
	jmp *%ecx
.size _longjmp,.-_longjmp
.size longjmp,.-longjmp
