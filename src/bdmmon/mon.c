#include	<stdio.h>
#include	<string.h>
#include        <sys/types.h>
#include        <sys/stat.h>
#include        <fcntl.h>
#include        <unistd.h>
#include        "sizedefs.h"
#include	"macro.h"
#include        "port.h"
#include	"status.h"
#include        <asm/io.h>
#include        "srec.h"
#include        "mc68332.h"

#define		MaxBuffer	128

int main()
{
  int lpt1;
  unsigned Counter, Counter1;
  int Stopped;
  char *ptr, Buffer[MaxBuffer], QuitFlag = '\0';
  char infile[MaxBuffer];
  LONG LongTemp, MemDump = 0, d0;
  unsigned int Adr, Data, stat;
  WORD tmp;
  BYTE c, flag;

  printf("Test Program for Background Mode Driver\n");

  lpt1 = open("/dev/port",O_RDWR);
  if(lpt1 < 0) {
    printf("Can't open port LPT1 (%d)\n",lpt1);
    exit(1);
  }

  printf("Initializing\n");
  bdm_init(0x378,100);

  printf("Resetting Target MCU to enable BDM\n");
  ResetChip();

  while (!QuitFlag) {
    printf("test-bdm>");
    fgets(Buffer,MaxBuffer-1,stdin);
    ptr = Buffer;
    switch (*ptr) {
      case 't':
	StopChip();
	break;
      case 'w':
	printf("Continue Execution\n");
	bdm_clk(0x0c00,17);
	break;
      case 'g':
	ptr++;
	sscanf(ptr, "%x", (unsigned int *)(&LongTemp));
        printf("Set PC to %x\n",(unsigned int)LongTemp);
        bdm_write(LongTemp,2,0x2480,(WORD)(LongTemp>>16),(WORD)(LongTemp&0xffff));
	LongTemp = 0x2000;
        bdm_write(LongTemp,2,0x248b,(WORD)(LongTemp>>16),(WORD)(LongTemp&0xffff));
	PutLong(0xfffe0a,0x0);  /* disable TPU */
	break;
      case 'z':
        c = inb(0x379);
	printf("R Port Status is %x\n",c);
        c = inb(0x37a);
	printf("W Port Status is %x\n",c);
	break;
      case 'c':
        stat = GetStatus();
        if (stat & TARGETNC) printf("Disconnected  ");
        else printf("Connected  ");
        if (stat & TARGETPOWER) printf("Power_off  ");
        else printf("Power_on  ");
        if (stat & TARGETRESET) printf("Reset  ");
        if (stat & TARGETSTOPPED) printf("Stopped\n");
        else printf("Running\n");        
        break;
      case 'y':
	ptr++;
	sscanf(ptr, "%d", &tmp);
        bdm_init(0x378,tmp);
	printf("Port Speed now: %d\n",tmp);
        break;
      case 'a':
	bdm_clk(0x2180,17);
        bdm_delay();
	tmp = bdm_clk(0x0,17);
	flag = (tmp & 0x10000) >> 15;
        d0 = (tmp & 0xffff) << 16;
	bdm_delay();
	tmp = bdm_clk(0x0,17);
	flag |= ((tmp & 0x10000) >> 16);
        d0 |= (tmp & 0xffff);
	printf("D0: %08x  flags %01x\n",(unsigned int)d0,flag);
	break;
      case 'p':
	ptr++;
	sscanf(ptr, "%x %x", &Adr,&Data);
        printf("Set adr %x to %x\n",Adr,Data);
	bdm_clk(0x1800,17);
        bdm_delay();
	tmp = bdm_clk((Adr>>16),17);
	flag = (tmp & 0x10000) >> 13;
 	bdm_delay();
	tmp = bdm_clk((Adr&0xffff),17);
	flag |= ((tmp & 0x10000) >> 14);
        bdm_delay();
	tmp = bdm_clk((Data&0xff),17);
	flag = (tmp & 0x10000) >> 15;
        bdm_delay();
	tmp = bdm_clk(0,17);
	flag = (tmp & 0x10000) >> 16;
	printf("flags %01x\n",flag);
	break;
      case 'b':
	RestartChip();
	break;
      case 'd':
	DumpRegisters();
	break;
      case 'h':
	printf("\tAvailable commands\n"
	       "\tb: Begin Program Execution from Reset\n"
	       "\td: Dump Target MCU Registers\n"
	       "\th: Print This Help Summary\n"
	       "\tl: Load srec into target\n"
               "\tf: Load srec into target flash\n"
               "\ti: Initialize CPU\n"
	       "\tm: Memory Hex/ASCII Display\n"
	       "\tr: Hardware Reset Target MCU\n"
	       "\ts: Single Step Target MCU\n"
	       "\tq: Quit back to DOS\n"
	       "\tt: Stop Chip\n"
	       "\tw: Send Go Command\n"
	       "\tg <value>: Start at address <value>\n"
	       "\tz: Display Port Status\n"
               "\tc: Get target status\n"
	       "\ty <value>: Set Speed for commands\n"
	       "\ta: Read D0\n"
               "\tp: set addr <adr> to <val>\n");
	break;

      case 'l':
	printf("S-Record File to Load: ");
	if (!fgets(Buffer, MaxBuffer-1,stdin)) {
          printf("Input not correct\r\n");
        }
        else {
          sscanf(Buffer,"%s",infile);
          do_load(0,0xffffff, infile);
        }
	break;

      case 'i':
        printf("Stop chip\n");
        StopChip();

        printf("Prepare CPU\n");
	PutLong(0xfffe0a,0x0);  /* disable TPU */
        PutWord(CPURAMCTRL_MCR,0x0100);  /* enable access to CPURAM */
        PutWord(CPURAMCTRL_BAR,0xf200);  /* set CPURAM base addr */
	PutWord(SIM_CSPAR0,0x2fff);     // cs5 is 8-bit port
	PutWord(SIM_CSPAR1,0x03fd);     // all others are 16-bit
	                                // enable A19
	PutWord(SIM_CSBARBT,0xe007);    // Rom at 0xe00000
	PutWord(SIM_CSORBT,0x7df0);     // allsorts
        PutWord(SIM_CSBAR0,0x0007);     // HI RAM
	PutWord(SIM_CSOR0,0x5c3e);      // base addr 0x0
	PutWord(SIM_CSBAR1,0x0007);     // LO RAM
	PutWord(SIM_CSOR1,0x3c3e);      // base addr 0x0
	PutWord(SIM_CSBAR5,0xffd8);     // DUART
	PutWord(SIM_CSOR5,0x7ffb);      // base addr 0xffd800
        break;

      case 'f':
        printf("Stop chip\n");
        StopChip();

        printf("Prepare CPU\n");
	PutLong(0xfffe0a,0x0);  /* disable TPU */
        PutWord(CPURAMCTRL_MCR,0x0100);  /* enable access to CPURAM */
        PutWord(CPURAMCTRL_BAR,0xf200);  /* set CPURAM base addr */
	PutWord(SIM_CSPAR0,0x2fff);     // cs5 is 8-bit port
	PutWord(SIM_CSPAR1,0x03fd);     // all others are 16-bit
	                                // enable A19
	PutWord(SIM_CSBARBT,0xe007);    // Rom at 0xe00000
	PutWord(SIM_CSORBT,0x7df0);     // allsorts
        PutWord(SIM_CSBAR0,0x0007);     // HI RAM
	PutWord(SIM_CSOR0,0x5c3e);      // base addr 0x0
	PutWord(SIM_CSBAR1,0x0007);     // LO RAM
	PutWord(SIM_CSOR1,0x3c3e);      // base addr 0x0
	PutWord(SIM_CSBAR5,0xffd8);     // DUART
	PutWord(SIM_CSOR5,0x7ffb);      // base addr 0xffd800
        
        printf("S-Record File to Load: ");
	if (!fgets(Buffer, MaxBuffer-1,stdin)) {
          printf("Input not correct\r\n");
        }
        else {
          sscanf(Buffer,"%s",infile);
          LongTemp = do_load(0,0xfffff,infile);
        }

        printf("Download flash macro\n");        
        do_load(0,0xffffff,"flash.srec");

        printf("Set download size (%ld Bytes)\n",LongTemp);        
	PutLong(CPURAM_BASE,LongTemp);      // size of the kernel

        printf("Set PC to 0xf2018a\n");
        LongTemp = 0xf2018a;
        bdm_write(LongTemp,2,0x2480,(WORD)(LongTemp>>16),(WORD)(LongTemp&0xffff));
	LongTemp = 0x2000;
        bdm_write(LongTemp,2,0x248b,(WORD)(LongTemp>>16),(WORD)(LongTemp&0xffff));
        printf("Starting Flash macro\n");
	bdm_clk(0x0c00,17);
	break;

      case 'm':
	printf("Memory Dump start address in hex:");
	fgets(Buffer, MaxBuffer-1,stdin);
	if (!sscanf(Buffer, "%lx", &LongTemp)) LongTemp = MemDump;
	Stopped = StopChip();
	set_fc();
	for (Counter=0; Counter<16; Counter++) {
	  printf("%08lX ", LongTemp);
	  for (Counter1=0; Counter1<16; Counter1++)
	    printf("%02X ", (BYTE) GetByte(LongTemp++));
	  printf("\n");
	}
	MemDump = LongTemp;
	restore_fc();
	break;

      case 'r':
	printf("Resetting Target MCU\n");
	ResetChip();
	break;

      case 's':
	printf("Single Step\n");
	StepChip();
	DumpRegisters();
	break;

      case 'q':
	QuitFlag = 1;
    }
  }
  bdm_deinit();
  close(lpt1);
  return 0;
}
