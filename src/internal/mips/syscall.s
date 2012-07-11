.set    noreorder

.global __syscall
.type   __syscall,@function
__syscall:
	move    $10, $4
	move    $4, $5
	move    $5, $6
	move    $6, $7
	lw      $7, 16($sp)
	lw      $8, 20($sp)
	lw      $9, 24($sp)
	subu    $sp, $sp, 32
	sw      $8, 16($sp)
	sw      $9, 20($sp)
	move    $2, $10
	syscall
	beq     $7, $0, 1f
	addu    $sp, $sp, 32
	subu    $2, $0, $2
1:	jr      $ra
	nop
