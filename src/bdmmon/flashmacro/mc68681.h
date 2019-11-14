/* mc68681.h -- Motorola mc68681 DUART register offsets. */

#define DUART_ADDR(nr) (DUART_BASE + nr)

#define DUART_MR1A	DUART_ADDR(0x00)  // Mode Register A 
#define DUART_MR2A	DUART_ADDR(0x00)  // Mode Register A 
#define DUART_SRA	DUART_ADDR(0x01)  // Status Register A 
#define DUART_CSRA	DUART_ADDR(0x01)  // Clock-Select Register A 
#define DUART_CRA	DUART_ADDR(0x02)  // Command Register A 
#define DUART_RBA	DUART_ADDR(0x03)  // Receive Buffer A 
#define DUART_TBA	DUART_ADDR(0x03)  // Transmit Buffer A 
#define DUART_IPCR	DUART_ADDR(0x04)  // Input Port Change Register 
#define DUART_ACR	DUART_ADDR(0x04)  // Auxiliary Control Register 
#define DUART_ISR	DUART_ADDR(0x05)  // Interrupt Status Register 
#define DUART_IMR	DUART_ADDR(0x05)  // Interrupt Mask Register 
#define DUART_CUR	DUART_ADDR(0x06)  // Counter Mode: current MSB 
#define DUART_CTUR	DUART_ADDR(0x06)  // Counter/Timer upper reg 
#define DUART_CLR	DUART_ADDR(0x07)  // Counter Mode: current LSB 
#define DUART_CTLR	DUART_ADDR(0x07)  // Counter/Timer lower reg 
#define DUART_MR1B	DUART_ADDR(0x08)  // Mode Register B 
#define DUART_MR2B	DUART_ADDR(0x08)  // Mode Register B 
#define DUART_SRB	DUART_ADDR(0x09)  // Status Register B 
#define DUART_CSRB	DUART_ADDR(0x09)  // Clock-Select Register B 
#define DUART_CRB	DUART_ADDR(0x0A)  // Command Register B 
#define DUART_RBB	DUART_ADDR(0x0B)  // Receive Buffer B 
#define DUART_TBB	DUART_ADDR(0x0B)  // Transmit Buffer A 
#define DUART_IVR	DUART_ADDR(0x0C)  // Interrupt Vector Register 
#define DUART_IP	DUART_ADDR(0x0D)  // Input Port 
#define DUART_OPCR	DUART_ADDR(0x0D)  // Output Port Configuration Reg. 
#define DUART_STRTCC	DUART_ADDR(0x0E)  // Start-Counter command 
#define DUART_OPRSET	DUART_ADDR(0x0E)  // Output Port Reg, SET bits 
#define DUART_STOPCC	DUART_ADDR(0x0F)  // Stop-Counter command 
#define DUART_OPRRST	DUART_ADDR(0x0F)  // Output Port Reg, ReSeT bits 
