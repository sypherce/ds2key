	.TEXT
	.ARM
	.ALIGN
 
.GLOBAL isRunningOnHW
isRunningOnHW:
    ldr r0,__test
    str r0,[pc,#-4]
    mov r0,#1
    bx lr
__test:
    mov r0,#0
