/* bdm-pd.c - routines to talk to CPU16 or CPU32 target * via
   public-domain interface hardware on IBM PC parallel printer port *
   Copyright (C) 1992 by Scott Howard, all rights reserved * Permission is 
   hereby granted to freely copy and use this code * or derivations
   thereof as long as no charge is made to anyone for its use */

#include        <sys/types.h>
#include        <unistd.h>
#include        <stdio.h>
#include        <asm/io.h>
#include        "sizedefs.h"
#include	"bdmcmd.h"
#include	"status.h"
#include	"bdm-pd.h"

/* bdm_deinit is called before program quits or goes to DOS * un-do any
   bad things which have been done to talk to target */

#define _inp() inb(bdm_port + 1)
#define _inpctrl() inb(bdm_port + 2)
#define _outp(val) outb(val,bdm_port + 2)

extern int ioperm(unsigned long, unsigned long, int);
extern int iopl(int);

extern void bdm_error(int);

unsigned int bdm_speed;
unsigned short bdm_port;
int old_ctl;
char RegsValid;

/* bdm_delay waits before returning to the calling routine */

void bdm_delay(void)
{
  int i, j;

  for (i = 0; i < bdm_speed; i++)
    j = i;
}

void bdm_deinit(void)
{
  int err;

  _outp(no_step);
  err = iopl(0);
}

/* bdm_init initializes parallel port to talk to target */

void bdm_init(int port, int speed)
{
  int err;

  err = iopl(3);
  if(err) {
    printf("No permission to access port\n");
    exit(1);
  }

  RegsValid = 0;
  old_ctl = _inp();
  _outp(no_step);
  bdm_port = port;
  bdm_speed = speed;
}

/* GetStatus returns status of MCU */

unsigned GetStatus(void)
{
  BYTE temp = _inp();

  if (!(temp & nc))
    return TARGETNC;
  if (!(temp & pwr_dwn))
    return TARGETPOWER;
  return (temp & freeze ? TARGETSTOPPED : 0)
    | (_inpctrl() & reset ? TARGETRESET : 0);
}

/* GetStatusMask returns mask showing which stat bits are valid */

unsigned GetStatusMask(void)
{
  return TARGETRESET | TARGETSTOPPED | TARGETPOWER | TARGETNC;
}

void ResetChip(void)
{
  RegsValid = 0;
  _outp(no_step | reset | bkpt);
  bdm_delay();
  _outp(no_step);
}

int RestartChip(void)
{
  unsigned long LoopCount;

  RegsValid = 0;
  _outp(no_step | reset | bkpt);
  bdm_delay();
  _outp(no_step | bkpt);
  bdm_delay();
  for (LoopCount = 50; LoopCount; LoopCount--) {
    if (_inp() & freeze)
      break;
    bdm_delay();
  }
  _outp(no_step);
  if (!LoopCount) return(BDM_FAULT_RESPONSE);
  else return(BDM_OK);
}

int StopChip(void)
{
  unsigned long LoopCount;

  if (_inp() & freeze) return (1);

  RegsValid = 0;
  _outp(no_step | bkpt);
  bdm_delay();
  for (LoopCount = 50; LoopCount; LoopCount--) {
    if (_inp() & freeze) break;
    bdm_delay();
  }
  if (!LoopCount) return(BDM_FAULT_RESPONSE);
  else return(BDM_OK);
}

/* bdm_clk sends <value> to MCU for <count> bits, returns MCU response */

LONG bdm_clk(WORD value, int count)
{
  LONG ShiftRegister = ((LONG) value) << (32 - count);
  unsigned char DataOut;
  unsigned stat = GetStatus();

  if (stat & TARGETRESET)
    printf("Target in reset state\n");
  if (stat & TARGETNC)
    printf("Target not connected?\n");
  if (stat & TARGETPOWER)
    printf("Target VCC low\n");

  while (count--) {
    DataOut = (ShiftRegister & 0x80000000) ? dsi : 0;
    ShiftRegister <<= 1;
    if (!(_inp() & dso))
      ShiftRegister |= 1;
    _outp(no_step | DataOut | neg_dsck);
    bdm_delay();
    _outp(no_step | DataOut);
    bdm_delay();
  }
  return ShiftRegister;
}

/* StepChip sends GO command word, then triggers breakpoint on first fetch */

int StepChip(void)
{
#define	DataOut	(BDM_GO & 1 ? dsi : 0)
  unsigned stat = GetStatus();
  int LoopCount;

  RegsValid = 0;
  if (stat & TARGETRESET)
    printf("Target in reset state\n");
  if (stat & TARGETNC)
    printf("Target not connected?\n");
  if (stat & TARGETPOWER)
    printf("Target VCC low\n");
  bdm_clk(BDM_GO >> 1, CommandBitCount - 1);
  _outp(no_step | neg_dsck | DataOut);
  bdm_delay();
  _outp(no_step| DataOut);
  _outp(0);
  for (LoopCount = 50; LoopCount; LoopCount--) {
    if (_inp() & freeze) break;
    bdm_delay();
  }
  RegsValid = 0;
  if (!LoopCount) return(BDM_FAULT_RESPONSE);
  else return(BDM_OK);
}

