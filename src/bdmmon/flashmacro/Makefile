.SUFFIXES: .srec .bin .coff .c .o
.PRECIOUS: .o .bin 

ARCH=m68k-coff-

CC=$(ARCH)gcc
AR=$(ARCH)ar
LD=$(ARCH)ld
OBJCOPY=$(ARCH)objcopy
LD_OPT=-nostdinc
CFLAGS=-Wall -msoft-float -mnobitfield
OBJS=e_handle.o sys_var.o crt0.o flash.o
OBJ=e_handle.o sys_var.o flash.o
HDR=asm.h mc68681.h global_def.h mc68332.h sys_var.h


all: flash.bin flash.srec

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.S.o:
	$(CC) $(CFLAGS) -c $< -o $@

flash.coff: $(OBJS) $(HDR) flash.ld
	$(LD) $(LD_OPT) -Map $*.map -o $@ $(OBJ) -T flash.ld

.coff.bin:
	$(OBJCOPY) -O binary $< $@

.coff.srec:
	$(OBJCOPY) -O srec $< $@

clean:
	\rm flash.bin flash.coff flash.srec *.o
