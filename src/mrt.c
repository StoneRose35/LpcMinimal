#include "types.h"
#include "mrt.h"
#include "chip.h"

volatile uint32_t mrt_counter;

void resetTimer()
{

*SYSAHBCLKCTRL |= (0x1<<10);
*PRESETCTRL &= ~(0x1<<7);
*PRESETCTRL |= (0x1<<7);

}

void initTimer()
{
    mrt_counter = 0;
	*NVIC_ISER0 = (1 << ((uint32_t)(MRT_IRQN) & 0x1F));

}

void setDelay0(uint32_t delay)
{
	*(MRT_INTVAL0) = delay;
	*(MRT_INTVAL0) |= 0x1UL<<31;
	*(MRT_CTRL0) = 1;
}

void setDelay1(uint32_t delay)
{
	*(MRT_INTVAL1) = delay;
	*(MRT_INTVAL1) |= 0x1UL<<31;
	*(MRT_CTRL1) = 1;
}


void runTimer(uint32_t delay)
{
	mrt_counter = 0;
	while (mrt_counter < delay)
	{

	}
}
