.set noreorder

.section .init
.global _init
_init:
	subu $sp,$sp,32
	sw $gp,24($sp)
	sw $ra,28($sp)

.section .fini
.global _fini
_fini:
	subu $sp,$sp,32
	sw $gp,24($sp)
	sw $ra,28($sp)
