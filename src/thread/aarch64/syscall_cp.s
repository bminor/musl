// __syscall_cp_asm(&self->cancel, nr, u, v, w, x, y, z)
//                  x0             x1  x2 x3 x4 x5 x6 x7

// syscall(nr, u, v, w, x, y, z)
//         x8  x0 x1 x2 x3 x4 x5

.global __syscall_cp_asm
.type __syscall_cp_asm,%function
__syscall_cp_asm:
.global __cp_begin
__cp_begin:
	ldr w0,[x0]
	cbnz w0,1f
	mov x8,x1
	mov x0,x2
	mov x1,x3
	mov x2,x4
	mov x3,x5
	mov x4,x6
	mov x5,x7
	svc 0
.global __cp_end
__cp_end:
	ret

	// cbnz might not be able to jump far enough
1:	b __cancel
