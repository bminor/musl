.syntax unified

.text
.global __tlsdesc_static
.hidden __tlsdesc_static
.type __tlsdesc_static,%function
__tlsdesc_static:
	ldr r0,[r0]
	bx lr

.hidden __tls_get_new

.global __tlsdesc_dynamic
.hidden __tlsdesc_dynamic
.type __tlsdesc_dynamic,%function
__tlsdesc_dynamic:
	push {r2,r3,ip,lr}
	ldr r1,[r0]
	ldr r2,[r1,#4]  // r2 = offset
	ldr r1,[r1]     // r1 = modid

	ldr r0,1f
	add r0,r0,pc
	ldr r0,[r0]
2:
#if __ARM_ARCH >= 5
	blx r0          // r0 = tp
#else
	mov lr,pc
	bx r0
#endif
	ldr r3,[r0,#-4] // r3 = dtv
	ldr ip,[r3]     // ip = dtv slot count
	cmp r1,ip
	bhi 3f
	ldr ip,[r3,r1,LSL #2]
	sub r0,ip,r0
	add r0,r0,r2    // r0 = r3[r1]-r0+r2
4:
#if __ARM_ARCH >= 5
	pop {r2,r3,ip,pc}
#else
	pop {r2,r3,ip,lr}
	bx lr
#endif

3:
#if __ARM_PCS_VFP || !__SOFTFP__
	vpush {d0-d7}
#endif
	push {r0-r3}
	add r0,sp,#4
	bl __tls_get_new
	pop {r1-r3,ip}
#if __ARM_PCS_VFP || !__SOFTFP__
	vpop {d0-d7}
#endif
	sub r0,r0,r1    // r0 = retval-tp
	b 4b

	.align 2
1:	.word __a_gettp_ptr - 2b
