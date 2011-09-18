.global __syscall_cp_asm
.type __syscall_cp_asm,%function
__syscall_cp_asm:
	mov ip,sp
	stmfd sp!,{r4,r5,r6,r7,lr}
	stmfd sp!,{r0}
	bl 1f
1:	mov r4,#(1f-.)
	add r4,r4,lr
	str r4,[r0,#4]
	str sp,[r0]
	mov r7,r1
	mov r0,r2
	mov r1,r3
	ldmfd ip,{r2,r3,r4,r5,r6}
1:	svc 0
	ldmfd sp!,{r1}
	mov r2,#0
	str r2,[r1]
	ldmfd sp!,{r4,r5,r6,r7,lr}
	tst lr,#1
	moveq pc,lr
	bx lr
