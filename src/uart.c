#include "uart.h"
#include "types.h"
#include "chip.h"


void resetUart0()
{
    // System Clock is apparently 60 MHz due to the PLL used, divide by two to get 30MHz base
	*UARTCLKDIV = 0x2;
	// set clock to uart0 and reset it
	*SYSAHBCLKCTRL |= (0x1<<14);
    *PRESETCTRL &= ~(0x1<<3);
	*PRESETCTRL |= (0x1<<3);

	// set fractional clock divider to UARTCLKDIV/(1 + 0/256) to get exact timing for Midi
	// and a tolerable error for the console
    *UARTFRGDIV = 0xff;
    *UARTFRGMULT = 0x00;
}

void resetUart1()
{
    // System Clock is apparently 60 MHz due to the PLL used, divide by two to get 30MHz base
	*UARTCLKDIV = 0x2;
	// set clock to uart1 and reset it
	*SYSAHBCLKCTRL |= (0x1<<15);
    *PRESETCTRL &= ~(0x1<<4);
	*PRESETCTRL |= (0x1<<4);

	// set fractional clock divider to UARTCLKDIV/(1 + 0/256) to get exact timing for Midi
	// and a tolerable error for the console
    *UARTFRGDIV = 0xff;
    *UARTFRGMULT = 0x00;
}

void initConsoleUart()
{

	// PIO0_4 as output
    *DIR0 |= 0x1 << 4;


    // disable interrupts while configuring
    *NVIC_ICER0 = (1 << ((uint32_t)(UART0_IRQN) & 0x1F));

    // route to PIO0_0 (RX)
    // route to PIO0_4 (TX)
    *PINASSIGN0 = 0xFFFF0004;

    // 8 data bits
    *UARTCFG0 |= (0x1 << 2);


    // set the baud rate generator to 195
    *BRG0 = 0xc3-0x1;


    //clear the status
    *USART_STAT0 |= (0x1 << 5) | (0x1 << 11);

	// enable Interrupt on receive
    *INTENSET0 = (0x1<<0);

    // enable interrupts
    *NVIC_ISER0 = (1 << ((uint32_t)(UART0_IRQN) & 0x1F));

	// enable
	*UARTCFG0 |= (0x1 << 0);

}

void initMidiUart()
{


    // disable interrupts while configuring
    *NVIC_ICER0 = (1 << ((uint32_t)(UART1_IRQN) & 0x1F));

    // route to PIO0_2 (RX)
    *PINASSIGN1 = 0xFF02FFFF;

    // 8 data bits
    *UARTCFG1 |= (0x1 << 2);


    // set the baud rate generator to 60
    *BRG1 = 0x3c-0x1;


    //clear the status
    *USART_STAT1 |= (0x1 << 5) | (0x1 << 11);

	// enable Interrupt on receive
    *INTENSET1 = (0x1<<0);

    // enable interrupts
    *NVIC_ISER0 = (1 << ((uint32_t)(UART1_IRQN) & 0x1F));

	// enable
	*UARTCFG1 |= (0x1 << 0);

}
