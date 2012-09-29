.global __set_thread_area
.type   __set_thread_area,@function
__set_thread_area:
	rtsd     r15, 8
	ori      r21, r5, 0
