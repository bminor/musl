.text
.global __clone
.type __clone, %function
__clone:
# int clone(fn, stack, flags, arg, ptid, tls, ctid)
#            a  b       c     d     e    f    g
#            3  4       5     6     7    8    9
# pseudo C code:
# tid = syscall(SYS_clone,c,b,e,f,g);
# if (!tid) syscall(SYS_exit, a(d));
# return tid;

# SYS_clone = 120
# SYS_exit = 1

# in order that the child can find the start func and its arg, we need to store it into
# non-volative regs. to do so, we have to store those 2 regs into our stackframe, so
# we can restore them later.
stw 30, -4(1)
stw 31, -8(1)
subi 1, 1, 16 

# save r3 (func) into r30, and r6(arg) into r31
mr 30, 3
mr 31, 6

#move c into first arg
mr 3, 5
#mr 4, 4
mr 5, 7
mr 6, 8
mr 7, 9

# move syscall number into r0    
li 0, 120

sc

# check for syscall error
#this code should be more efficient, but it borks
#bns+ 1f # jump to label 1 if no summary overflow.
#else
#neg 3, 3 #negate the result (errno)
#b 2f # jump to epilogue

# this error check code at least does not spoil the clone call.
#mfcr    0                      # Check for an error
#rlwinm  4, 0, 0, 3, 3        # by checking for bit 28.
#cmplwi  0, 4, 0               # It is an error if non-zero.
#beq     0, 1f                  # Jump if not an error.
#neg     3, 3                  # Negate the error number.
#b       2f # jump to epilogue
1:
# compare sc result with 0
cmpwi cr7, 3, 0

# if not 0, jump to end
bne cr7, 2f

#else: we're the child
#call funcptr
# move arg (d) into r3
mr 3, 31
#move r30 (funcptr) into CTR reg
mtctr 30
# call CTR reg
bctrl
# mov SYS_exit into r0 (the exit param is already in r3)
li 0, 1
sc

2:

# restore stack
addi 1, 1, 16
lwz 30, -4(1)
lwz 31, -8(1)

blr




