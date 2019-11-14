/* 
 * this file defines the types BYTE, WORD, and LONG to declare types
 * that match the data sizes used in the target microcontroller(s)
 * BYTE is one byte, WORD is two bytes, LONG is 4 bytes
 * these are all unsigned quantities
 * change these definitions if you are using a compiler with different 
 * default sizes
 */

#ifndef	SIZEDEFS_H

#define	SIZEDEFS_H
#define	BYTE	unsigned char
#define	WORD	unsigned int
#define	LONG	unsigned long

#endif

/* end of sizedefs.h */
