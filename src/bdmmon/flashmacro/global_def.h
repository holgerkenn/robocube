#define EPROM_BASE       0xf00000  // -- 0xf1ffff
#define CPURAM_BASE      0xf20000  // -- 0xf207ff
#define DUART_BASE       0xffd800  // -- 0xffdfff

#define SIM_BASE         0xfffa00
#define CPURAMCTRL_BASE  0xfffb00
#define SCI_BASE         0xfffc00
#define TPU_BASE         0xfffe00

#define readbyte(x)    (*(volatile char *)x)
#define writebyte(x,y) *(char *)(x) = y
#define readshort(x)    (*(short *)(void *)(x))
#define writeshort(x,y) *(short *)(x) = y
#define readlong(x)    (*(unsigned long *)(void *)(x))
#define writelong(x,y) *(unsigned long *)(x) = y

#define writeshortpos(x,y,mask,pos) writeshort(x,(readshort(x)\
				      &(~((mask)<<(pos)))) \
				      |(((mask)&y)<<(pos)))
/* Writes y at x, shifted left by pos, masked with mask */
/* example writeshortpos(TPU_CFSR3,0x9,0xf,0x8) */
/*  writes function code 0x9 (4 bits wide, so mask 0xf) into TPU */
/*  channel function select register for TPU Channel 2 (bits 8-11) */  

#define readshortpos(x,mask,pos) ((readshort(x)>>(pos))&(mask))
/* This reads... */
