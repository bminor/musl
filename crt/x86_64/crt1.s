/* Written 2011 Nicholas J. Kain, released as Public Domain */
.text
.global _start
_start:
	xor %rbp,%rbp   /* rbp:undefined -> mark as zero 0 (ABI) */
	mov %rdx,%r9    /* 6th arg: ptr to register with atexit() */
	pop %rsi        /* 2nd arg: argc */
	mov %rsp,%rdx   /* 3rd arg: argv */
	andq $-16,%rsp  /* align stack pointer */
	push %rax       /* 8th arg: glibc ABI compatible */
	push %rsp       /* 7th arg: glibc ABI compatible */
	xor %r8,%r8     /* 5th arg: always 0 */
	xor %rcx,%rcx   /* 4th arg: always 0 */
	mov $main,%rdi  /* 1st arg: application entry ip */
	call __libc_start_main /* musl init will run the program */
.L0:	jmp .L0
