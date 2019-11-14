#include "asm.h"
#include "global_def.h"
#include "mc68332.h"
#include "mc68681.h"

	.title "crt0.S for m68k-coff"

/*
   ROM/RAM size: 3=64k, 4=128k, 5=256k, 6=512k, 7=1M
*/

#define ROMTYPE 	4 
#define RAMTYPE         7
#define INITSTACK 	0x20000 // Stack location after reset (never used)

	.text

	.extern SYM(exp_handler)
	.extern SYM(flash_write)

#include "ivtable.S"

start:
	move.b	SIM_SYPCR,d0	//
	or.b	IMM(6),d0	// bus mon enab, bus mon timeout 16 sys cycles
	and.b	IMM(0x7f),d0	// disable watchdog
	move.b	d0,SIM_SYPCR	//
	move.w	IMM(0x2700),sr	// supv mode, level 7 int mask
	or.w	IMM(0x4000),SIM_SYNCR	// 16.8MHz clock speed
clockwait:
	move.w	SIM_SYNCR,d0	// wait for clock to stabilise
	cmp.w	IMM(0x7f08),d0	//
	bne	clockwait	//

	move.w 	IMM(0x604f),SIM_MCR

	move.l	IMM(IVTABLE),d0   //
	movec	d0,vbr		  // set vector base register

/* set up initial stack frame */
      	movel	IMM(__stack), sp
	link	a6, IMM(-8)

init_duart:	
        move.b  IMM(0),DUART_IMR
        move.b  IMM(0),DUART_OPCR
        move.b  IMM(0x2a),DUART_CRA
        move.b  IMM(0x2a),DUART_CRB
        move.b  IMM(0x3a),DUART_CRA
        move.b  IMM(0x3a),DUART_CRB
        move.b  IMM(0x60),DUART_ACR
        move.b  IMM(0),DUART_CTUR
        move.b  IMM(2),DUART_CTLR
        move.b  DUART_STRTCC,d0
        move.b  IMM(0xbb),DUART_CSRA
        move.b  IMM(0xdd),DUART_CSRB
        move.b  IMM(0x13),DUART_MR1A
        move.b  IMM(0x13),DUART_MR1B
        move.b  IMM(7),DUART_MR2A
        move.b  IMM(7),DUART_MR2B
        move.b  IMM(0xff),DUART_OPRRST
        move.b  IMM(5),DUART_CRA
        move.b  IMM(5),DUART_CRB

duartwait:
        move.b  DUART_SRB,d0    // wait for duart to finish
        and.b   IMM(0x4),d0     //
        beq     duartwait       //

	jmp flash_write

halt:
	jmp halt


