#define WEAK __attribute__ ((weak))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

/* direct Interrupt Handler declarations*/
     void ResetISR(void);
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void SVC_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);
WEAK void IntDefaultHandler(void);

/*aliased interrupt handler declarations*/
void SPI0_IRQHandler(void) ALIAS(IntDefaultHandler);
void SPI1_IRQHandler(void) ALIAS(IntDefaultHandler);
void UART0_IRQHandler(void) ALIAS(IntDefaultHandler);
void UART1_IRQHandler(void) ALIAS(IntDefaultHandler);
void UART2_IRQHandler(void) ALIAS(IntDefaultHandler);
void I2C_IRQHandler(void) ALIAS(IntDefaultHandler);
void SCT_IRQHandler(void) ALIAS(IntDefaultHandler);
void MRT_IRQHandler(void) ALIAS(IntDefaultHandler);
void CMP_IRQHandler(void) ALIAS(IntDefaultHandler);
void WDT_IRQHandler(void) ALIAS(IntDefaultHandler);
void BOD_IRQHandler(void) ALIAS(IntDefaultHandler);
void WKT_IRQHandler(void) ALIAS(IntDefaultHandler);
void PININT0_IRQHandler(void) ALIAS(IntDefaultHandler);
void PININT1_IRQHandler(void) ALIAS(IntDefaultHandler);
void PININT2_IRQHandler(void) ALIAS(IntDefaultHandler);
void PININT3_IRQHandler(void) ALIAS(IntDefaultHandler);
void PININT4_IRQHandler(void) ALIAS(IntDefaultHandler);
void PININT5_IRQHandler(void) ALIAS(IntDefaultHandler);
void PININT6_IRQHandler(void) ALIAS(IntDefaultHandler);
void PININT7_IRQHandler(void) ALIAS(IntDefaultHandler);

extern int main(void);

/*symbols from linker*/
extern void _vStackTop(void);
WEAK extern void __valid_user_code_checksum();


extern void (* const g_pfnVectors[])(void);
__attribute__ ((used,section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    // Core Level - CM0plus
    &_vStackTop, // The initial stack pointer
    ResetISR,                               // The reset handler
    NMI_Handler,                            // The NMI handler
    HardFault_Handler,                      // The hard fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    __valid_user_code_checksum,             // LPC MCU Checksum
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    SVC_Handler,                            // SVCall handler
    0,                                      // Reserved
    0,                                      // Reserved
    PendSV_Handler,                         // The PendSV handler
    SysTick_Handler,                        // The SysTick handler

    // Chip Level - LPC8xx
    SPI0_IRQHandler,                         // SPI0 controller
    SPI1_IRQHandler,                         // SPI1 controller
    0,                                       // Reserved
    UART0_IRQHandler,                        // UART0
    UART1_IRQHandler,                        // UART1
    UART2_IRQHandler,                        // UART2
    0,                                       // Reserved
    0,                                       // Reserved
    I2C_IRQHandler,                          // I2C controller
    SCT_IRQHandler,                          // Smart Counter Timer
    MRT_IRQHandler,                          // Multi-Rate Timer
    CMP_IRQHandler,                          // Comparator
    WDT_IRQHandler,                          // Watchdog
    BOD_IRQHandler,                          // Brown Out Detect
    0,                                       // Reserved
    WKT_IRQHandler,                          // Wakeup timer
    0,                                       // Reserved
    0,                                       // Reserved
    0,                                       // Reserved
    0,                                       // Reserved
    0,                                       // Reserved
    0,                                       // Reserved
    0,                                       // Reserved
    0,                                       // Reserved
    PININT0_IRQHandler,                      // PIO INT0
    PININT1_IRQHandler,                      // PIO INT1
    PININT2_IRQHandler,                      // PIO INT2
    PININT3_IRQHandler,                      // PIO INT3
    PININT4_IRQHandler,                      // PIO INT4
    PININT5_IRQHandler,                      // PIO INT5
    PININT6_IRQHandler,                      // PIO INT6
    PININT7_IRQHandler,                      // PIO INT7
}; /* End of g_pfnVectors */


/*
functions used to copy memory blocks
*/
__attribute__ ((section(".after_vectors")))
void data_init(unsigned int romstart, unsigned int start, unsigned int len) {
    unsigned int *pulDest = (unsigned int*) start;
    unsigned int *pulSrc = (unsigned int*) romstart;
    unsigned int loop;
    for (loop = 0; loop < len; loop = loop + 4)
        *pulDest++ = *pulSrc++;
}

__attribute__ ((section(".after_vectors")))
void bss_init(unsigned int start, unsigned int len) {
    unsigned int *pulDest = (unsigned int*) start;
    unsigned int loop;
    for (loop = 0; loop < len; loop = loop + 4)
        *pulDest++ = 0;
}

/*
linker symbols at the start and end of the memory blocks
*/
extern unsigned int __data_section_table;
extern unsigned int __data_section_table_end;
extern unsigned int __bss_section_table;
extern unsigned int __bss_section_table_end;


/*
Start function declared as such in the linker script, this is where it all starts...
*/
__attribute__ ((section(".after_vectors")))
void
ResetISR(void) {

    
    //
    // Copy the data sections from flash to SRAM.
    //
    unsigned int LoadAddr, ExeAddr, SectionLen;
    unsigned int *SectionTableAddr;

    // Load base address of Global Section Table
    SectionTableAddr = &__data_section_table;

    // Copy the data sections from flash to SRAM.
    while (SectionTableAddr < &__data_section_table_end) {
        LoadAddr = *SectionTableAddr++;
        ExeAddr = *SectionTableAddr++;
        SectionLen = *SectionTableAddr++;
        data_init(LoadAddr, ExeAddr, SectionLen);
    }
    // At this point, SectionTableAddr = &__bss_section_table;
    // Zero fill the bss segment
    while (SectionTableAddr < &__bss_section_table_end) {
        ExeAddr = *SectionTableAddr++;
        SectionLen = *SectionTableAddr++;
        bss_init(ExeAddr, SectionLen);
    }
    

    main();

    //
    // main() shouldn't return, but if it does, we'll just enter an infinite loop
    //
    while (1) {
        ;
    }
}


__attribute__ ((section(".after_vectors")))
void IntDefaultHandler(void)
{
    while(1)
    {
    }
}

__attribute__ ((section(".after_vectors")))
void NMI_Handler(void)
{
    while(1)
    {
    }
}

__attribute__ ((section(".after_vectors")))
void HardFault_Handler(void)
{
    while(1)
    {
    }
}


