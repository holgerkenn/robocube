STARTUP(crt0.o)
OUTPUT_ARCH(m68k)
/*OUTPUT_FORMAT(srec)*/

GROUP(
/usr/local/lib/gcc-lib/m68k-coff/2.8.1/m68000/msoft-float/libgcc.a
)

PROVIDE (__DYNAMIC  =  0);

/*
 * Setup the memory map of the M68332 bootmonitor
 * stack grows down from CPU memory; bss section grows up from CPU memory
 *
 * The memory map look like this:
 * +--------------------+ <- 0x0
 * | .text              |
 * |        _etext      |
 * |        ctor list   | the ctor and dtor lists are for
 * |        dtor list   | C++ support
 * +--------------------+ <- cpu memory 0xf20000
 * | .data              | initialized data goes here
 * |        _edata      |
 * +--------------------+
 * | .bss               |
 * |        __bss_start | start of bss, cleared by crt0
 * |        _end        | start of heap, used by sbrk()
 * +--------------------+
 * .                    .
 * .                    .
 * .                    .
 * |        __stack     | top of stack
 * +--------------------+ <- end of cpu memory 0xf207fc
 */
MEMORY
{
  rom : ORIGIN = 0xf00000, LENGTH = 128K
  cpuram : ORIGIN = 0xf20000, LENGTH = 2048
}

/*
 * allocate the stack to be at the top of memory, since the stack
 * grows down
 */

PROVIDE (__stack = 0xf207fc);

/*
 * stick text in rom (of course)
 * variables in ram
 */
SECTIONS
{
  .text :
  {
    *(.text)
    . = ALIGN(0x4);
  } > cpuram

  .data :
  {
    __data_start = . ;
    *(.shdata)
    *(.data)
    _edata = .;
  } > cpuram

  .bss :
  {
    . = ALIGN(0x4);
    __bss_start = . ;
    *(.shbss)
    *(.bss)
    *(COMMON)
    _end =  ALIGN (0x8);
    __end = _end;
  } > cpuram

  .stab 0 (NOLOAD) :
  {
    *(.stab)
  }

  .stabstr 0 (NOLOAD) :
  {
    *(.stabstr)
  }
}


