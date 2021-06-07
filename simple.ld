
/* stripped down linker script containing only the relevant symbols and definitions*/

/*Entry Point for the Kernel */
ENTRY(ResetISR)


MEMORY
{
  /* Define each memory region */
  MFlash4 (rx) : ORIGIN = 0x0, LENGTH = 0x1000 /* 4K bytes (alias Flash) */  
  RamLoc1 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x400 /* 1K bytes (alias RAM) */  
}

  /* Define a symbol for the top of each memory region */
  __top_RamLoc1 = 0x10000000 + 0x400 ; /* 1K bytes */ 
  
SECTIONS
{
    /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        /* Global Section Table */
        . = ALIGN(4) ; 
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        __bss_section_table_end = .;
        __section_table_end = . ;
	    /* End of Global Section Table */

        *(.after_vectors*)

    } >MFlash4
    
    .data : ALIGN(4)
    {
       FILL(0xff)
       _data = . ;
       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
    } > RamLoc1 AT>MFlash4
    
    
    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(end = .);
    } > RamLoc1
    
    PROVIDE(_vStackTop = __top_RamLoc1 - 0);
    
        PROVIDE(__valid_user_code_checksum = 0 - 
                                         (_vStackTop 
                                         + (ResetISR + 1) 
                                         + (NMI_Handler  + 1) 
                                         + (HardFault_Handler + 1) 
                                         )
           );
}
