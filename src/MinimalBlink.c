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


void setPwmValue(char);
void handleNoteOff(void);


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
   
    phase += TONE_FREQ/SAMPLING_FREQ*0xFFFFFFFFUL;

    wave = phase >> (32 - DAC_RES); // output a aliased sawtooth
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
	
    while(1) { // "OS"-Loop
    	*SET0 = 0x1 << 2;
        runTimer(1000);
        *CLR0 = 0x1 << 2; 
        runTimer(1000);
    }
    return 0 ;
}
