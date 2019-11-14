/* Exception/Interrupt vector table -----------------------------------------*/

IVTABLE:
	.long   0       // Reset system stack location. (not used)
	dc.l    SYM(start)      // Reset start address.
	dc.l    BUSSERR         // Buss error handler.
	dc.l    ADRSERR         // Address error handler.
	dc.l    ILLEGALINSTR    // Illigal instruction.
	dc.l    ZERODIVIDE      // Zero divide.
	dc.l    CHKS            // CHK, CHK2 instructions.
	dc.l    TRAPCCV         // TRAPcc, TRAPV instructions.
	dc.l    PRIVVIOLATED    // Privalage violation.
	dc.l    TRACE           // Trace.
	dc.l    UNUSEDVECT      // Line 1010 emulator.
	dc.l    UNUSEDVECT      // Line 1111 emulator.
	dc.l    HRDWAREBKPT     // Hardware breakpoint.
	dc.l    UNUSEDVECT      // Reserved (Motorola).
	dc.l    UNINITIALISED   // Format error and uninitialised interrupt.
	dc.l    UNINITIALISED   // Format error and uninitialised interrupt.
	.rept 8
	.long UNUSEDVECT    // All unused vectors.
	.endr
	dc.l    SPURIOUSINT     // Spuriouse interrupt.
	dc.l    AUTOVECT1       // Level 1 interrupt autovector.
	dc.l    AUTOVECT2       // Level 2 interrupt autovector.
	dc.l    AUTOVECT3       // Level 3 interrupt autovector.
	dc.l    AUTOVECT4       // Level 4 interrupt autovector.
	dc.l    AUTOVECT5       // Level 5 interrupt autovector.
	dc.l    AUTOVECT6       // Level 6 interrupt autovector.
	dc.l    AUTOVECT7       // Level 7 interrupt autovector.

BUSSERR:
	move.w  #1,__errno
	jmp     SYM(exp_handler)
ADRSERR:
	move.w  #2,__errno
	jmp     SYM(exp_handler)
ILLEGALINSTR:
	move.w  #3,__errno
	jmp     SYM(exp_handler)
ZERODIVIDE:
	move.w  #4,__errno
	jmp     SYM(exp_handler)
CHKS:
	move.w  #5,__errno
	jmp     SYM(exp_handler)
TRAPCCV:
	move.w  #6,__errno
	jmp     SYM(exp_handler)
PRIVVIOLATED:
	move.w  #7,__errno
	jmp     SYM(exp_handler)
TRACE:
	move.w  #8,__errno
	jmp     SYM(exp_handler)
UNUSEDVECT:
	move.w  #9,__errno
	jmp     SYM(exp_handler)
HRDWAREBKPT:
	move.w  #10,__errno
	jmp     SYM(exp_handler)
UNINITIALISED:
	move.w  #11,__errno
	jmp     SYM(exp_handler)
SPURIOUSINT:
	move.w  #12,__errno
	jmp     SYM(exp_handler)
AUTOVECT1:
	move.w  #13,__errno
	jmp     SYM(exp_handler)
AUTOVECT2:
	move.w  #14,__errno
	jmp     SYM(exp_handler)
AUTOVECT3:
	move.w  #15,__errno
	jmp     SYM(exp_handler)
AUTOVECT4:
	move.w  #16,__errno
	jmp     SYM(exp_handler)
AUTOVECT5:
	move.w  #17,__errno
	jmp     SYM(exp_handler)
AUTOVECT6:
	move.w  #18,__errno
	jmp     SYM(exp_handler)
AUTOVECT7:
	move.w  #19,__errno
	jmp     SYM(exp_handler)

