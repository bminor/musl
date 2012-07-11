.set noreorder
.global __clone
.type   __clone,@function
__clone:
	# Save function pointer and argument pointer
	move $25, $4
	move $8, $7
	# Shuffle (fn,sp,fl,arg,ptid,tls,ctid) to (sp,fl,ptid,tls,ctid)
	move $4, $5
	move $5, $6
	lw $6, 16($sp)
	lw $7, 20($sp)
	lw $9, 24($sp)
	sw $9, 16($sp)
	li $2, 4120
	syscall
	beq $7, $0, 1f
	nop
	jr $ra
	subu $2, $0, $2
1:	beq $2, $0, 1f
	nop
	jr $ra
	nop
1:	move $4, $8
	jr $25
	nop
