#include        <stdarg.h>
#include        <stdio.h>
#include        "sizedefs.h"
#include	"bdmcmd.h"
#include	"port.h"
#include        "status.h"

static WORD RegCodes[] =
{
  BDM_WSREG + BDM_RPC, BDM_WSREG + BDM_USP,
  BDM_WSREG + BDM_SSP, BDM_WSREG + BDM_VBR,
  BDM_WSREG + BDM_SR, BDM_WSREG + BDM_SFC,
  BDM_WSREG + BDM_DFC, BDM_WSREG + BDM_ATEMP,

  BDM_WRREG + 0, BDM_WRREG + 1, BDM_WRREG + 2, BDM_WRREG + 3,
  BDM_WRREG + 4, BDM_WRREG + 5, BDM_WRREG + 6, BDM_WRREG + 7,

  BDM_WRREG + 8, BDM_WRREG + 9, BDM_WRREG + 10, BDM_WRREG + 11,
  BDM_WRREG + 12, BDM_WRREG + 13, BDM_WRREG + 14, BDM_WRREG + 15
};

static void bdm_clrerror(void);
static BYTE fc_set = 0, fc = 5;
static LONG old_sfc, old_dfc;
static char last_rw;
static LONG last_addr;
char RegsValid;

void bdm_error(int type);
void bdm_clrerror(void);
int set_fc(void);
int restore_fc(void);


LONG bdm_read(LONG addr, int pcount, WORD cmd,...)
{
  va_list arg;
  int err, count;
  LONG response, result;

  last_addr = addr;
  last_rw = 1;
  result = 0L;
  for (err = 3; err; err--) {
    count = pcount;
    va_start(arg, cmd);
    response = bdm_clk(cmd, CommandBitCount);
    while (count--)
      response = bdm_clk(va_arg(arg, WORD), CommandBitCount);
      if (response > BDM_NOTREADY) {
	if (response == BDM_BERR)
	  bdm_error(BDM_FAULT_BERR);
	continue;
      }
    for (count = (cmd & BDM_LONGSIZE) ? 2 : 1; count; count--) {
      while ((response = bdm_clk(BDM_NOP, CommandBitCount)) == BDM_NOTREADY);
      if (response < BDM_NOTREADY) {
	result <<= 16;
	result |= response;
      }
      else {
	if (response == BDM_BERR)
	  bdm_error(BDM_FAULT_BERR);
	break;
      }
    }
    if (response > BDM_NOTREADY) {
      if (response == BDM_BERR)
	bdm_error(BDM_FAULT_BERR);
      bdm_clrerror();
      continue;
    }
    va_end(arg);
    break;
  }
  if (!err)
    bdm_error(BDM_FAULT_UNKNOWN);
  response = bdm_clk(BDM_NOP, CommandBitCount);
  if (response == BDM_BERR)
    bdm_error(BDM_FAULT_BERR);
  return result;
}

void bdm_write(LONG addr, int pcount, WORD cmd,...)
{
  va_list arg;
  int err, count;
  LONG response;

  last_addr = addr;
  last_rw = 0;
  for (err = 3; err; err--) {
    count = pcount;
    va_start(arg, cmd);
    response = bdm_clk(cmd, CommandBitCount);
    while (count--)
      response = bdm_clk(va_arg(arg, WORD), CommandBitCount);
      if (response > BDM_NOTREADY) {
	if (response == BDM_BERR)
	  bdm_error(BDM_FAULT_BERR);
	bdm_clrerror();
	continue;
      }
    while ((response = bdm_clk(BDM_NOP, CommandBitCount)) == BDM_NOTREADY);
    if (response == BDM_CMDCMPLTE)
      break;
    else if (response == BDM_BERR)
      bdm_error(BDM_FAULT_BERR);

  }
  va_end(arg);
  if (!err)
    bdm_error(BDM_FAULT_UNKNOWN);
  response = bdm_clk(BDM_NOP, CommandBitCount);
  if (response == BDM_BERR)
    bdm_error(BDM_FAULT_BERR);
}

void RunChip(LONG where)
{
  StopChip();
  if (where)
    bdm_write(where, 2, BDM_WSREG + BDM_RPC, (WORD) (where >> 16), (WORD) where);
  bdm_clk(BDM_GO, CommandBitCount);
}

LONG GetByte(LONG x)
{
  int save;
  LONG result;

  save = set_fc();
  result = bdm_read(x, 2, BDM_READ, (WORD) (x >> 16), (WORD) x);
  if (save)
    restore_fc();
  return result;
}

void PutByte(LONG x, BYTE y)
{
  int save;

  save = set_fc();
  bdm_write(x, 3, BDM_WRITE, (WORD) (x >> 16), (WORD) x, (WORD) y);
  if (save)
    restore_fc();
}

void FillByte(LONG x, BYTE y)
{
  int save;

  save = set_fc();
  bdm_write(x, 1, BDM_FILL, (WORD) y);
  if (save)
    restore_fc();
}

LONG DumpByte(LONG x)
{
  return GetByte(x);
}

LONG GetWord(LONG x)
{
  int save;
  LONG result;

  save = set_fc();
  result = bdm_read(x, 2, BDM_READ + BDM_WORDSIZE, (WORD) (x >> 16), (WORD) x);
  if (save)
    restore_fc();
  return result;
}

void PutWord(LONG x, WORD y)
{
  int save;

  save = set_fc();
  bdm_write(x, 3, BDM_WRITE + BDM_WORDSIZE, (WORD) (x >> 16), (WORD) x, y);
  if (save)
    restore_fc();
}

void FillWord(LONG x, WORD y)
{
  int save;

  save = set_fc();
  bdm_write(x, 1, BDM_FILL + BDM_WORDSIZE, y);
  if (save)
    restore_fc();
}

LONG DumpWord(LONG x)
{
  return GetWord(x);
}

LONG GetLong(LONG x)
{
  int save;
  LONG result;

  save = set_fc();
  result = bdm_read(x, 2, BDM_READ + BDM_LONGSIZE, (WORD) (x >> 16), (WORD) x);
  if (save)
    restore_fc();
  return result;
}

void PutLong(LONG x, LONG y)
{
  int save;

  save = set_fc();
  bdm_write(x, 4, BDM_WRITE + BDM_LONGSIZE, (WORD) (x >> 16), (WORD) x, (WORD) (y >> 16), (WORD) y);
  if (save)
    restore_fc();
}

void FillLong(LONG x, LONG y)
{
  int save;

  save = set_fc();
  bdm_write(x, 2, BDM_FILL + BDM_LONGSIZE, (WORD) (y >> 16), (WORD) y);
  if (save)
    restore_fc();
}

LONG DumpLong(LONG x)
{
  return GetLong(x);
}

LONG GetReg(unsigned which)
{
  LONG result;

  result = bdm_read(0L, 0, BDM_RDBIT + RegCodes[which]);
  return result;
}

void PutReg(unsigned which, LONG data)
{
  bdm_write(0L, 2, RegCodes[which], (WORD) (data >> 16), (WORD) data);
}

void DumpRegisters(void)
{
  unsigned SR, Counter;
  LONG Temp;

  StopChip();
  printf("D0-7 ");
  for (Counter = 0; Counter < 8; Counter++)
    printf("%08lx ", GetReg(REG_D0 + Counter));
  printf("\n");

  printf("A0-7 ");
  for (Counter = 0; Counter < 8; Counter++)
    printf("%08lx ", GetReg(REG_A0 + Counter));
  printf("\n");

  printf("PC   %08lx      SR  ", GetReg(REG_PC));
  Temp = GetReg(REG_SR);
  SR = 0x8000;
  for (Counter = 0; Counter < 16; Counter++) {
    if(Temp & SR) printf("1");
    else printf("0");
    SR >>= 1;
  }
  printf("\nSFC  %08lx      DFC %08lx      VBR %08lx\n",
	 GetReg(REG_SFC), GetReg(REG_DFC), GetReg(REG_VBR));
}

int restore_fc(void)
{
  if (!fc_set)
    return 0;
  PutReg(REG_SFC, (LONG) old_sfc);
  PutReg(REG_DFC, (LONG) old_dfc);
  fc_set = 0;
  return 1;
}

void SetFC(unsigned NewFC)
{
  if (NewFC == UserData || NewFC == UserCode
      || NewFC == SupervisorData || NewFC == SupervisorCode)
    fc = NewFC;
}

int set_fc(void)
{
  if (fc_set)
    return 0;
  old_sfc = (int)GetReg(REG_SFC);
  old_dfc = (int)GetReg(REG_DFC);
  PutReg(REG_SFC, (LONG) fc);
  PutReg(REG_DFC, (LONG) fc);
  return fc_set = 1;
}

void bdm_error(int type)
{
  if (type == BDM_FAULT_BERR)
    restore_fc();
}

void bdm_clrerror(void)
{
  bdm_clk(BDM_NOP, CommandBitCount);
  bdm_clk(BDM_NOP, CommandBitCount);
  bdm_clk(BDM_NOP, CommandBitCount);
  bdm_clk(BDM_NOP, CommandBitCount);
  while (bdm_clk(0, 1));
  while (!bdm_clk(0, 1));
  bdm_clk(0, 15);
}

