.set noreorder

.global feclearexcept
.type  feclearexcept,@function
feclearexcept:
	cfc1    $5, $31
	or      $5, $5, $4
	xor     $5, $5, $4
	ctc1    $5, $31
	jr      $ra
	li      $2, 0

.global feraiseexcept
.type  feraiseexcept,@function
feraiseexcept:
	cfc1    $5, $31
	or      $5, $5, $4
	ctc1    $5, $31
	jr      $ra
	li      $2, 0

.global fetestexcept
.type  fetestexcept,@function
fetestexcept:
	cfc1    $2, $31
	jr      $ra
	and     $2, $2, $4

.global fegetround
.type  fegetround,@function
fegetround:
	cfc1    $2, $31
	jr      $ra
	andi    $2, $2, 3

.global fesetround
.type  fesetround,@function
fesetround:
	cfc1    $5, $31
	li      $6, -4
	and     $5, $5, $6
	or      $5, $5, $4
	jr      $ra
	li      $2, 0

.global fegetenv
.type  fegetenv,@function
fegetenv:
	cfc1    $5, $31
	sw      $5, 0($4)
	jr      $ra
	li      $2, 0

.global fesetenv
.type  fesetenv,@function
fesetenv:
	lw      $5, 0($4)
	ctc1    $5, $31
	jr      $ra
	li      $2, 0
