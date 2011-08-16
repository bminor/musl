.text
.global dlsym
.type dlsym,@function
dlsym:
	mov (%rsp),%edx
	jmp __dlsym
