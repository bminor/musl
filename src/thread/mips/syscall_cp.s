.set    noreorder

.global __syscall_cp_asm
.type   __syscall_cp_asm,@function
__syscall_cp_asm:
	subu    $sp, $sp, 32
.global __cp_begin
__cp_begin:
	lw      $4, 0($4)
	bne     $4, $0, __cp_cancel
	move    $2, $5
	move    $4, $6
	move    $5, $7
	lw      $6, 48($sp)
	lw      $7, 52($sp)
	lw      $8, 56($sp)
	lw      $9, 60($sp)
	lw      $10,64($sp)
	sw      $8, 16($sp)
	sw      $9, 20($sp)
	sw      $10,24($sp)
	sw      $2, 28($sp)
	lw      $2, 28($sp)
	syscall
.global __cp_end
__cp_end:
	beq     $7, $0, 1f
	addu    $sp, $sp, 32
	subu    $2, $0, $2
1:	jr      $ra
	nop

.global __cp_cancel
__cp_cancel:
	addu    $sp, $sp, 32
	lw      $25, %call16(__cancel)($gp)
	jr      $25
	nop
