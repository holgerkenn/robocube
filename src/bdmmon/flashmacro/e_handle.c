#include "sys_var.h"
#include "mc68681.h"
#include "global_def.h"

  char *e_name[20] = {"UNK","BUS ERR","ADR ERR",
                      "ILL",
                      "DIV/0","CHKS","TRAPCCV",
                      "PRIV","TRC","UNUSED",
                      "BKPT","UNINIT",
                      "SPURIOUS","AVEC1","AVEC2",
                      "AVEC3","AVEC4","AVEC5",
                      "AVEC6","AVEC7"
		      };
  char *intro = "\r\n*ERROR*\r\n";
  char *halt = "\r\n*HALT*\r\n";
  char *ptr;

void exp_handler(void)
{

  asm(" ori.w #0x0700,%sr"); /* disable all further interupts */

  if(__errno>35) __errno = 0;

  ptr = intro;
  while (*ptr != '\0') {
    while(!(readbyte(DUART_SRB) & 4));
    writebyte(DUART_TBB,*ptr++);
  }

  ptr = e_name[__errno];
  while (*ptr != '\0') {
    while(!(readbyte(DUART_SRB) & 4));
    writebyte(DUART_TBB,*ptr++);
  }

  ptr = halt;
  while (*ptr != '\0') {
    while(!(readbyte(DUART_SRB) & 4));
    writebyte(DUART_TBB,*ptr++);
  }

  while (1);
}
