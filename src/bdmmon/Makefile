.SUFFIXES: .dl .bin .coff .c .o
.PRECIOUS: .o .bin 

ARCH=

CC=$(ARCH)gcc
AR=$(ARCH)ar
LD=$(ARCH)ld
LD_OPT=
CFLAGS=-Wall -O2
OBJS=port.o macro.o mon.o srec.o
HDR=bdm-pd.h bdmcmd.h macro.h port.h sizedefs.h srec.h status.h mc68332.h

all: mon

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

mon: $(OBJS) $(HDR)
	$(CC) -o $@ $(OBJS)

clean:
	\rm *.o mon
