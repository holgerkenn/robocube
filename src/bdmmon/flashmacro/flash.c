#include "mc68681.h"
#include "global_def.h"

#define FLASH_BASE 0xe00000
#define CPURAM_BASE 0xf20000
#define RAM_BASE 0x0

char *erasing = "\r\nErasing Flash\r\n";
char *writing = "Writing Flash from RAM\r\n";
char *verify = "Verifying\r\n";
char *finish = "\tfinished successfully\r\n";
char *fail = "\tfailed\r\n";
char *p, ok;
unsigned short int tmp1, tmp2;
unsigned long size, addr;

void flash_write(void)
{

  p = erasing;
  while (*p != '\0') {
    while(!(readbyte(DUART_SRB) & 4));
    writebyte(DUART_TBB,*p++);
  }

  writeshort(FLASH_BASE+(0x555<<1),0xaaaa);
  writeshort(FLASH_BASE+(0x2aa<<1),0x5555);
  writeshort(FLASH_BASE+(0x555<<1),0x8080);
  writeshort(FLASH_BASE+(0x555<<1),0xaaaa);
  writeshort(FLASH_BASE+(0x2aa<<1),0x5555);
  writeshort(FLASH_BASE+(0x555<<1),0x1010);
  while(~readshort(FLASH_BASE));

  p = writing;
  while (*p != '\0') {
    while(!(readbyte(DUART_SRB) & 4));
    writebyte(DUART_TBB,*p++);
  }

  size = readlong(CPURAM_BASE);
  addr = 0;

  while(addr < size) {
    tmp1 = readshort(RAM_BASE+addr);
    writeshort(FLASH_BASE+(0x555<<1),0xaaaa);
    writeshort(FLASH_BASE+(0x2aa<<1),0x5555);
    writeshort(FLASH_BASE+(0x555<<1),0xa0a0);
    writeshort(FLASH_BASE+addr,tmp1);

    ok = 0;
    while(!ok) {
      tmp2 = readshort(FLASH_BASE+addr);
      if ((tmp1 ^ tmp2) & 0x8080) {   //not finished
        if (tmp2 & 0x2020) {          //time out ?
          tmp2 = readshort(FLASH_BASE+addr);
          if ((tmp1 ^ tmp2) & 0x8080) {   //yes!          
            p = fail;
            while (*p != '\0') {
              while(!(readbyte(DUART_SRB) & 4));
              writebyte(DUART_TBB,*p++);
            }
            while(1);
          }
          ok = 1;
        }
      }
      else ok = 1;
    }
    addr += 2;
  }

  p = verify;
  while (*p != '\0') {
    while(!(readbyte(DUART_SRB) & 4));
    writebyte(DUART_TBB,*p++);
  }

  addr = 0;

  while(addr < size) {
    tmp1 = readshort(RAM_BASE+addr);
    tmp2 = readshort(FLASH_BASE+addr);
    if(tmp1 ^ tmp2) {  //verify failed
      p = fail;
      while (*p != '\0') {
        while(!(readbyte(DUART_SRB) & 4));
        writebyte(DUART_TBB,*p++);
      }
      while(1);
    }
    addr += 2;
  }

  p = finish;
  while (*p != '\0') {
    while(!(readbyte(DUART_SRB) & 4));
    writebyte(DUART_TBB,*p++);
  }

  while(1);

}
