#define	bdm_ctl         2	/* offset of control port from base */
#define	no_step         8	/* set low to gate IFETCH onto BKPT */
#define	dsi             4	/* data shift input - PC->MCU       */
#define	reset           2	/* low when RESET asserted          */
#define	neg_dsck        1	/* neg. data shift clock            */
#define bkpt            1       /* breakpoint asserted (low)        */

#define	bdm_stat	1	/* offset of status port from base     */
#define	nc              0x80	/* not connected - low when unplugged  */
#define	pwr_dwn         0x40	/* power down - low when Vcc failed    */
#define	dso             0x20	/* data shift output - MCU->PC         */
#define	freeze          8	/* FREEZE asserted when MCU stopped    */
