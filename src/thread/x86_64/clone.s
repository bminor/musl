/* Copyright 2011 Nicholas J. Kain, licensed GNU LGPL 2.1 or later */
.text
.global __uniclone
.type   __uniclone,%function
/* rdi = child_stack, rsi = start, rdx = pthread_struct */
__uniclone:
        subq    $8,%rsp         /* pad parent stack to prevent branch later */
        subq    $16,%rdi        /* grow child_stack */
        mov     %rsi,8(%rdi)    /* push start onto child_stack as return ptr */
        mov     %rdx,0(%rdi)    /* push pthread_struct onto child_stack */
        mov     %rdx,%r8        /* r8 = tls */
        mov     %rdi,%rsi       /* rsi = child_stack */
        leaq    40(%rdx),%r10   /* r10 = child_id */
        movl    $56,%eax        /* clone syscall number */
        movl    $0x7d0f00,%edi  /* rdi = flags */
        mov     %r10,%rdx       /* rdx = parent_id */
        syscall                 /* clone(flags, child_stack, parent_id,
                                 *       child_id, tls) */
        pop     %rdi            /* child stack: restore pthread_struct
                                 * parent stack: undo rsp displacement */
        ret
.size __uniclone,.-__uniclone
