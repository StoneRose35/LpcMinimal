/*
===============================================================================
 Name        : MinimalBlink.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "types.h"
#include "chip.h"
#include "synth.h"


void setPwmValue(char);
void handleNoteOff(void);
uint32_t sineVal(uint32_t);


volatile uint32_t mrt_counter;
volatile uint32_t phase;
volatile unsigned short wave;
volatile char amplitude;

/*midi controller variables*/
volatile char midiNotes = 0;
volatile char midiCntr = 0;
volatile char playedNote = 0;
volatile char lastStatus = 0x80;
volatile char noteBfr;
volatile char notes[16];

const uint32_t phaseIncrements[120] = { 0x00217057, 0x00236d5d, 0x002588a8, 0x0027c404, 0x002a215a, 0x002ca2af, 0x002f4a26, 0x00321a04, 0x003514b1, 0x00383cb8, 0x003b94ca, 0x003f1fc5, 0x0042e0ae, 0x0046daba, 0x004b1150, 0x004f8809, 0x005442b4, 0x0059455e, 0x005e944c, 0x00643409, 0x006a2963, 0x00707970, 0x00772995, 0x007e3f8a, 0x0085c15c, 0x008db575, 0x009622a1, 0x009f1012, 0x00a88569, 0x00b28abc, 0x00bd2899, 0x00c86813, 0x00d452c6, 0x00e0f2e0, 0x00ee532b, 0x00fc7f15, 0x010b82b9, 0x011b6aeb, 0x012c4542, 0x013e2024, 0x01510ad3, 0x01651578, 0x017a5132, 0x0190d026, 0x01a8a58c, 0x01c1e5c0, 0x01dca657, 0x01f8fe2a, 0x02170572, 0x0236d5d6, 0x02588a84, 0x027c4048, 0x02a215a6, 0x02ca2af0, 0x02f4a264, 0x0321a04c, 0x03514b18, 0x0383cb81, 0x03b94cae, 0x03f1fc55, 0x042e0ae5, 0x046dabac, 0x04b11508, 0x04f88091, 0x05442b4c, 0x059455e0, 0x05e944c9, 0x06434099, 0x06a29630, 0x07079703, 0x0772995d, 0x07e3f8ab, 0x085c15ca, 0x08db5758, 0x09622a11, 0x09f10123, 0x0a885699, 0x0b28abc0, 0x0bd28992, 0x0c868132, 0x0d452c61, 0x0e0f2e07, 0x0ee532bb, 0x0fc7f157, 0x10b82b94, 0x11b6aeb1, 0x12c45422, 0x13e20246, 0x1510ad33, 0x16515780, 0x17a51325, 0x190d0264, 0x1a8a58c3, 0x1c1e5c0f, 0x1dca6576, 0x1f8fe2ae, 0x21705728, 0x236d5d63, 0x2588a844, 0x27c4048d, 0x2a215a67, 0x2ca2af01, 0x2f4a264a, 0x321a04c9, 0x3514b186, 0x383cb81e, 0x3b94caed, 0x3f1fc55c, 0x42e0ae51, 0x46dabac6, 0x4b115088, 0x4f88091b, 0x5442b4cf, 0x59455e02, 0x5e944c95, 0x64340992, 0x6a29630c, 0x7079703d, 0x772995db, 0x7e3f8ab8};
volatile uint32_t p_i;

void MRT_IRQHandler(void)
{
  if ( *MRT_STAT0 & MRT_STAT_INTFLAG )
  {
    *MRT_STAT0 = MRT_STAT_INTFLAG;      /* clear interrupt flag */
    mrt_counter++;
  }
  else if ( *MRT_STAT1 & MRT_STAT_INTFLAG ) 
  { /*sampling interrupt, compute next phase and the next waveform value and put it in dac*/
   /*increment is TONE_FREQ/SAMPLING_FREQ*RESOLUTION */
   
    phase += p_i; //TONE_FREQ/SAMPLING_FREQ*0xFFFFFFFFUL;

    //wave = phase >> (32 - DAC_RES); // output a aliased sawtooth
    uint32_t p_bfr = phase >> 16;
    wave = sineVal(p_bfr) >> 16 - DAC_RES;
    wave *= amplitude;
    wave = wave >> 8;
    setPwmValue(wave);
    *MRT_STAT1 = MRT_STAT_INTFLAG;
  }
  return;
}

void UART0_IRQHandler(void)
{
	char byteRead;
	if (*USART_STAT0 & RXRDY)
	{
		/*read midi message*/
		byteRead = *RXDAT0 & 0xFF;
		if (byteRead & 0x80) /*check for status byte*/
		{
			midiCntr=0;
			lastStatus = byteRead & 0xF0;
		}
		else
		{
			if (midiCntr==0)
			{
				noteBfr = byteRead;
				midiCntr++;
			}
			else if (midiCntr==1)
			{
				switch (lastStatus)
				{
					case MIDI_NOTEOFF:

						if (midiNotes > 0)
						{
							handleNoteOff();
						}

						break;
					case MIDI_NOTEON:
						if (byteRead == 0)
						{
							if (midiNotes > 0)
							{
								handleNoteOff();
							}
						}
						else
						{
							amplitude = 255;
							*(notes+midiNotes) = noteBfr;
							midiNotes++;
							/*set note, currently implementing highest first*/
							if (noteBfr > playedNote)
							{
								playedNote = noteBfr;
								p_i=phaseIncrements[playedNote];
							}

						}
						break;
					default:
						break;
				}
				midiCntr=0;
			}
		}

	}
}


void handleNoteOff(void)
{
	for (char c=0;c<midiNotes;c++)
	{
		if (*(notes+c)==noteBfr)
		{
			for(char cc=c;c<15;cc++)
			{
				*(notes+cc) = *(notes + cc + 1);
			}

			*(notes+15) = 0;
		}
	}
	midiNotes--;
	if (midiNotes > 0)
	{
		playedNote = *(notes + midiNotes-1);
	}
	else
	{
		playedNote=0;
		amplitude=0;
	}
}

void initTimer()
{
          mrt_counter = 0;
	  *SYSAHBCLKCTRL |= (0x1<<10);
	  *PRESETCTRL &= ~(0x1<<7);
	  *PRESETCTRL |= (0x1<<7);

	  *NVIC_ISER0 = (1 << ((uint32_t)(MRT_IRQN) & 0x1F));
	  
}

void setDelay(uint32_t delay,char channelNr) 
{
	*(MRT_INTVAL0 + channelNr*0x10) = delay; 
	*(MRT_INTVAL0 + channelNr*0x10) |= 0x1UL<<31; 
	*(MRT_CTRL0 +  + channelNr*0x10) = 1;
}

void runTimer(uint32_t delay)
{
	mrt_counter = 0;
	while (mrt_counter < delay)
	{

	}
}

void initPwmDac()
{
	*SYSAHBCLKCTRL |= (0x1<<8);
    *PRESETCTRL &= ~(0x1<<8);
	*PRESETCTRL |= (0x1<<8);
	
	// disable default function on PIO0_3
	*PINENABLE0 |= 0x1 << 2;
	
	// CTOUT_0_O to PIO0_3
	*PINASSIGN6 |= 0x3<<24UL;
	
	// SET OUTPUT 0 on Event 0 (PWM Compare match) and clear on event 1 (counter limit)
	*SCT_OUT0_SET |= 0x1 << 0; 
	*SCT_OUT0_CLR |= 0x1 << 1;
	
	// unify counters to get a single 32bit counter 
	*SCT_CONFIG |= 0x1 << SCT_UNIFY;
	
	// Set event 1 as limit event
	*SCT_LIMIT |= 0x1 << 1;
	
	// initialize counter with zero
	*SCT_COUNT = 0;
	
	// initialize to get zero analog voltage, i.e. generate a square wave on the output
	SCT_MATCH0->VAL = 0xFF >> 1;
	
	// initialize resolution 
	SCT_MATCH1->VAL = 0xFF  >> 8-DAC_RES;
	
	// Configuration for event 0: PWM Comparison match
	// Combmode match only 0x1 << 12, direction counting up 0x1 << 21, CNTOUT_0 is selected 0x1 << 5
	*SCT_EV0_CTRL |= (0x1 << 12) | (0x1 << 21) | (0x1 << 5);
	
	// Configuration for event 1: counter reset
	// Combmode match only 0x1 << 12, direction counting up 0x1 << 21, use match register 1 
	*SCT_EV1_CTRL |= (0x1 << 12) | (0x1 << 21) | (0x1 << 0);
	
	// set halt to zero to start the counter
	*SCT_CTRL &= ~(0x0 << 2); 
	
	phase = 0.0;
}

void initMidiUart()
{
	// set clock to uart0 and reset it
	*SYSAHBCLKCTRL |= (0x1<<14);
    *PRESETCTRL &= ~(0x1<<3);
	*PRESETCTRL |= (0x1<<3);

	// configure UART: Enable, 8 data bits, all other settings comply with the reset values
	*UARTCFG0 |= (0x1 << 0) | (0x1 << 2);

	// enable Interrupt on receive
    *INTENSET |= (0x1<<0);

    // route to PIO0_4 (RX)
    *PINASSIGN0 |= 0x4 << 8;

    // set clock divider to 16* the midi baud rate before the fractional divider
    *UARTCLKDIV = UART_CLOCK_DIV << 0;

    // set the fractional divider part to 0
    *UARTFRGMULT = 0x0;
    *UARTFRGDIV = 0xff;

    // set the baud rate generator to 1
    *BRG0 = 0x0;
}

void setPwmValue(char val)
{
	SCT_MATCHREL0->VAL = val;
}

void initGpio()
{
	// route clock to GPIO, reset GPIO
	*SYSAHBCLKCTRL |= (0x1<<6);
	*PRESETCTRL &= ~(0x1<<10);
	*PRESETCTRL |= (0x1<<10);


	// PIO0_2 as output
    *DIR0 |= 0x1 << 2;//0x4;

    // disable swd on pin2
    *PINENABLE0 |= 0x1 << 3; //0xFFFFFFFFUL;

}

uint32_t div(uint32_t dividend,uint32_t divisor)
{
	uint32_t bfr;
	uint32_t res=0;
	bfr=dividend;
	while(bfr > divisor)
	{
		res++;
		bfr -=divisor;
	}
	return res;
}

uint32_t sineTaylor(uint32_t p)
{
	uint32_t bfr;
	uint32_t res;
	bfr=p*p;
	bfr = bfr >> 16 & 0xFFFF;
	bfr = bfr*p;
	bfr = div(bfr,3170);
	//bfr /= 3170;
	res = (p*3 - bfr);
	return(res);
}

/*returns an approximated sine value with 16 bit resolution for both the phase value and and returned sine value
 * input and output are actually unsigned 16bit integer stored in a 32bit variable to save a few processing steps for typecasting
 * */
uint32_t sineVal(uint32_t phase)
{
	uint32_t res;
	if ((phase & 0x0000c000)  == 0)
	{
		res = sineTaylor(phase) + 0x7FFF;
	}
	else if ((phase & 0x0000c000) == 1 << 14)
	{
		uint32_t phase_rel = 0x7FFF - phase;
		res = 0x7FFF + sineTaylor(phase_rel);
	}
	else if ((phase & 0x0000c000) == 2 << 14)
	{
		uint32_t phase_rel = phase - 0x7FFF;
		res = 0x7FFF - sineTaylor(phase_rel);
	}
	else
	{
		uint32_t phase_rel = 0xFFFF - phase;
		res = 0x7FFF - sineTaylor(phase_rel);
	}

	return res;
	//taylor series: x - x^3/6
}


int main(void) {

        
	*SYSAHBCLKCTRL |= ( (0x1 << SWM) | (0x1 << IOCON) );
	
	// "driver" loading
	initGpio();
	initTimer();
	// set channel 0 as a led blinker
	setDelay(CLOCK_FREQ/1000,0);
	// set channel 1 as sample clock
	setDelay(CLOCK_FREQ/SAMPLING_FREQ,1);
	
	// init DAC 
	initPwmDac();
	//init Midi Interface
	initMidiUart();
	
    while(1) { // "OS"-Loop
    	*SET0 = 0x1 << 2;
        runTimer(1000);
        *CLR0 = 0x1 << 2; 
        runTimer(1000);
    }
    return 0 ;
}
