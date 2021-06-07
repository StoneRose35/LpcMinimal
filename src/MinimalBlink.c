/*
===============================================================================
 Name        : MinimalBlink.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/


#include "chip.h"


volatile uint32_t mrt_counter = 0;

void MRT_IRQHandler(void)
{
  if ( *MRT_STAT0 & MRT_STAT_INTFLAG )
  {
    *MRT_STAT0 = MRT_STAT_INTFLAG;      /* clear interrupt flag */
    mrt_counter++;
  }
  return;
}


void initTimer(uint32_t delay)
{
	  *SYSAHBCLKCTRL |= (0x1<<10);
	  *PRESETCTRL &= ~(0x1<<7);
	  *PRESETCTRL |= (0x1<<7);


	  mrt_counter = 0;
	  *MRT_INTVAL0 = delay;
	  *MRT_INTVAL0 |= 0x1UL<<31;

	  *MRT_CTRL0 = 1; //MRT_MODE_REPEAT;

	  *NVIC_ISER0 = (1 << ((uint32_t)(MRT_IRQN) & 0x1F));
	  
}

void runTimer(uint32_t delay)
{
	mrt_counter = 0;
	while (mrt_counter < delay)
	{

	}
}

void gpio_init()
{
	// route clock to GPIO, reset GPIO
	*SYSAHBCLKCTRL |= (0x1<<6);
	*PRESETCTRL &= ~(0x1<<10);
	*PRESETCTRL |= (0x1<<10);


	// PIO0_2 as output
    *DIR0 |= 0x1 << 2;//0x4;

	// disable swd on pin2
    *PINENABLE0 = 0xFFFFFFFFUL;

}


int main(void) {

	*SYSAHBCLKCTRL |= ( (0x1 << SWM) | (0x1 << IOCON) );

	gpio_init();
	initTimer(CLOCK_FREQ/1000);
    while(1) {
    	*SET0 = 0x1 << 2;
        runTimer(1000);
        *CLR0 = 0x1 << 2; 
        runTimer(1000);
    }
    return 0 ;
}
