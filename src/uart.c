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
}

void initMidiUart()
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


    // set the baud rate generator to 125
    *BRG0 = 0x7d-0x1;

    // set the fractional divider part to 144/256
    *UARTFRGDIV = 0xff;
    *UARTFRGMULT = 0x90;

    //clear the status
    *USART_STAT0 |= (0x1 << 5) | (0x1 << 11);

	// enable Interrupt on receive
    *INTENSET = (0x1<<0);

    // enable interrupts
    *NVIC_ISER0 = (1 << ((uint32_t)(UART0_IRQN) & 0x1F));

	// enable
	*UARTCFG0 |= (0x1 << 0);

}
