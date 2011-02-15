/* Copyright 2011 Nicholas J. Kain, licensed GNU LGPL 2.1 or later */
.text
.global __set_thread_area
.type   __set_thread_area,%function
__set_thread_area:
        push %rbx               /* save x86_64 abi clobbered registers */
        push %r11
        mov %rdi,%rsi           /* shift for syscall */
        movl $0x1002,%edi       /* SET_FS register */
        movl $158,%eax          /* set fs segment to */
        syscall                 /* arch_prctl(SET_FS, arg)*/
        pop %r11                /* restore clobbered registers */
        pop %rbx
	ret
.size __set_thread_area,.-__set_thread_area
