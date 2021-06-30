#include "types.h"
#include "chip.h"
#include "pwmdac.h"


void resetSCT()
{
	// reset and init SCT
	*SYSAHBCLKCTRL |= (0x1<<8);
    *PRESETCTRL &= ~(0x1<<8);
	*PRESETCTRL |= (0x1<<8);

}

void initPwmDac()
{


	// PIO0_3 as output
    *DIR0 |= 0x1 << 3;

	// disable default function on PIO0_3
	*PINENABLE0 |= 0x1 << 2;

	// unify counters to get a single 32bit counter
	*SCT_CONFIG |= (0x1 << SCT_UNIFY);

	// CTOUT_0_O to PIO0_3
	*PINASSIGN6 = 0x03FFFFFFUL;


	// initialize resolution
	SCT_MATCH0->REG_VAL = 0xFFUL >> 1;
	SCT_MATCHREL0->REG_VAL = 0xFFUL >> 1;

	// initialize to get zero analog voltage, i.e. generate a square wave on the output
	SCT_MATCH1->REG_VAL = 0xFFUL  >> 0;
	SCT_MATCHREL1->REG_VAL = 0xFFUL  >> 0;


	// enable event 0 in state 0
	*SCT_EV0_STATE = 0x1;
	// Configuration for event 0: pwm match
	// Combmode match only 0x1 << 12, match register 0
	*SCT_EV0_CTRL = (0x1 << 12) | (0x0 << 0);

	// Configuration for event 1: counter reset
	// Combmode match only 0x1 << 12, use match register 1
	// enable event 1 in state 0
	*SCT_EV1_STATE = 0x1;
	*SCT_EV1_CTRL = (0x1 << 12) | (0x1 << 0);

	// set zero in the state register
	*SCT_STATE = 0x0;

	// set OUTPUT 0 on Event 0 (counter limit) and clear on event 1 (pwm match)
	*SCT_OUT0_SET = 0x1 << 1;
	*SCT_OUT0_CLR = 0x1 << 0;

	// Set event 1 as limit event
	*SCT_LIMIT = 0x1 << 1;

	// set halt to zero to start the counter
	*SCT_CTRL &= ~(0x1 << 2);

}



void setPwmValue(uint32_t val)
{
	SCT_MATCHREL0->REG_VAL = val;
}
