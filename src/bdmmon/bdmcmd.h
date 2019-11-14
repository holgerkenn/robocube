/* define CPU32 register names and commands */

#define	REG_PC		0
#define	REG_USP		1
#define	REG_SSP		2
#define	REG_VBR		3
#define	REG_SR		4
#define	REG_SFC		5
#define	REG_DFC		6
#define	REG_ATEMP	7
#define	REG_D0		8
#define	REG_D1		(REG_D0 + 1)
#define	REG_D2		(REG_D0 + 2)
#define	REG_D3		(REG_D0 + 3)
#define	REG_D4		(REG_D0 + 4)
#define	REG_D5		(REG_D0 + 5)
#define	REG_D6		(REG_D0 + 6)
#define	REG_D7		(REG_D0 + 7)
#define	REG_A0		16
#define	REG_A1		(REG_A0 + 1)
#define	REG_A2		(REG_A0 + 2)
#define	REG_A3		(REG_A0 + 3)
#define	REG_A4		(REG_A0 + 4)
#define	REG_A5		(REG_A0 + 5)
#define	REG_A6		(REG_A0 + 6)
#define	REG_A7		(REG_A0 + 7)

#define	REG_MAX		REG_A7

#define BDM_RDREG       0x2180
#define BDM_WRREG       0x2080
#define BDM_RSREG       0x2580
#define BDM_WSREG       0x2480
#define BDM_READ        0x1900
#define BDM_WRITE       0x1800
#define BDM_DUMP        0x1D00
#define BDM_FILL        0x1C00
#define BDM_GO          0x0C00
#define BDM_CALL        0x0800
#define BDM_RST         0x0400
#define BDM_NOP         0x0000
	
#define BDM_BYTESIZE    0x00
#define BDM_WORDSIZE    0x40
#define BDM_LONGSIZE    0x80
#define BDM_RDBIT       0x100
	
#define BDM_RPC         0x0
#define BDM_PCC         0x1
#define BDM_SR          0xb
#define BDM_USP         0xc
#define BDM_SSP         0xd
#define BDM_SFC         0xe
#define BDM_DFC         0xf
#define BDM_ATEMP       0x8
#define BDM_FAR         0x9
#define BDM_VBR         0xa
	
#define BDM_NOTREADY    0x10000
#define BDM_BERR        0x10001
#define BDM_ILLEGAL     0x1FFFF
#define BDM_CMDCMPLTE   0x0FFFF

#define CommandBitCount 17

#define	UserData	1
#define	UserCode	2
#define	SupervisorData	5
#define	SupervisorCode	6
