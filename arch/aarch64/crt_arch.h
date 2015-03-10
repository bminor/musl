__asm__(
".global _start\n"
".type _start,%function\n"
"_start:\n"
"	mov x29, #0\n"
"	mov x30, #0\n"
"	mov x0, sp\n"
"	and sp, x0, #-16\n"
"	b __cstart\n");
