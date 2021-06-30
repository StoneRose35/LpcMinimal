#include "gpio.h"
#include "types.h"
#include "chip.h"


void resetGPIO()
{
	// route clock to GPIO, reset GPIO
	*SYSAHBCLKCTRL |= (0x1<<6);
	*PRESETCTRL &= ~(0x1<<10);
	*PRESETCTRL |= (0x1<<10);
}

void initGpio()
{

	// PIO0_2 as output
    *DIR0 |= 0x1 << 2;

    // disable swdio in pio0_2
    *PINENABLE0 |= (0x1 << 3);
}
