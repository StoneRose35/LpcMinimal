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
#include "midi.h"
#include "minilib.h"
#include "uart.h"
#include "mrt.h"
#include "gpio.h"
#include "pwmdac.h"
#include "adagio_for_tron.h"


extern uint32_t mrt_counter;
volatile uint32_t phase;
volatile uint16_t wave;
volatile uint8_t amplitude;
volatile uint8_t playedNote;




const uint32_t phaseIncrements[120] = { 0x00217057, 0x00236d5d, 0x002588a8, 0x0027c404, 0x002a215a, 0x002ca2af, 0x002f4a26, 0x00321a04, 0x003514b1, 0x00383cb8, 0x003b94ca, 0x003f1fc5, 0x0042e0ae, 0x0046daba, 0x004b1150, 0x004f8809, 0x005442b4, 0x0059455e, 0x005e944c, 0x00643409, 0x006a2963, 0x00707970, 0x00772995, 0x007e3f8a, 0x0085c15c, 0x008db575, 0x009622a1, 0x009f1012, 0x00a88569, 0x00b28abc, 0x00bd2899, 0x00c86813, 0x00d452c6, 0x00e0f2e0, 0x00ee532b, 0x00fc7f15, 0x010b82b9, 0x011b6aeb, 0x012c4542, 0x013e2024, 0x01510ad3, 0x01651578, 0x017a5132, 0x0190d026, 0x01a8a58c, 0x01c1e5c0, 0x01dca657, 0x01f8fe2a, 0x02170572, 0x0236d5d6, 0x02588a84, 0x027c4048, 0x02a215a6, 0x02ca2af0, 0x02f4a264, 0x0321a04c, 0x03514b18, 0x0383cb81, 0x03b94cae, 0x03f1fc55, 0x042e0ae5, 0x046dabac, 0x04b11508, 0x04f88091, 0x05442b4c, 0x059455e0, 0x05e944c9, 0x06434099, 0x06a29630, 0x07079703, 0x0772995d, 0x07e3f8ab, 0x085c15ca, 0x08db5758, 0x09622a11, 0x09f10123, 0x0a885699, 0x0b28abc0, 0x0bd28992, 0x0c868132, 0x0d452c61, 0x0e0f2e07, 0x0ee532bb, 0x0fc7f157, 0x10b82b94, 0x11b6aeb1, 0x12c45422, 0x13e20246, 0x1510ad33, 0x16515780, 0x17a51325, 0x190d0264, 0x1a8a58c3, 0x1c1e5c0f, 0x1dca6576, 0x1f8fe2ae, 0x21705728, 0x236d5d63, 0x2588a844, 0x27c4048d, 0x2a215a67, 0x2ca2af01, 0x2f4a264a, 0x321a04c9, 0x3514b186, 0x383cb81e, 0x3b94caed, 0x3f1fc55c, 0x42e0ae51, 0x46dabac6, 0x4b115088, 0x4f88091b, 0x5442b4cf, 0x59455e02, 0x5e944c95, 0x64340992, 0x6a29630c, 0x7079703d, 0x772995db, 0x7e3f8ab8};
const uint32_t* p_phaseIncrements = phaseIncrements;
volatile uint32_t p_i = phaseIncrements[60];

void MRT_IRQHandler(void)
{
  if ( *MRT_STAT0 & MRT_STAT_INTFLAG )
  {
    *MRT_STAT0 = MRT_STAT_INTFLAG;      /* clear interrupt flag */
    mrt_counter++;
  }

  if ( *MRT_STAT1 & MRT_STAT_INTFLAG )
  { //sampling interrupt, compute next phase and the next waveform value and put it in dac
   //increment is TONE_FREQ/SAMPLING_FREQ*RESOLUTION

    phase += p_i; //TONE_FREQ/SAMPLING_FREQ*0xFFFFFFFFUL;

    wave = phase >> (32 - DAC_RES); // output a aliased sawtooth
    uint32_t p_bfr = phase >> 16;
    //wave = sineVal(p_bfr) >> 16 - DAC_RES;
    wave *= amplitude;
    wave = wave >> 8;
    setPwmValue(wave);
    *MRT_STAT1 = MRT_STAT_INTFLAG;
    //*NOT0 |= 0x1 << 3;
  }
  return;
}

void UART0_IRQHandler(void)
{
	uint8_t byteRead;
	if (*USART_STAT0 & RXRDY)
	{
		/*read console data*/
		byteRead = *RXDAT0 & 0xFF;

		//simple echo
		sendChar(byteRead);
	}
	return;
}

void UART1_IRQHandler(void)
{
	uint8_t byteRead;
	if (*USART_STAT1 & RXRDY)
	{
		/*read midi message*/
		byteRead = *RXDAT1 & 0xFF;
		printf("midi: ");
	    char dbfr[11];
        for(uint8_t c=0;c<11;c++)
        {
        	*(dbfr + c) = 0;
        }
        asHex(byteRead,dbfr);
        printf(dbfr);
        printf("\r\n");
		handleMidiNote(byteRead,&playedNote,&amplitude);
		p_i = phaseIncrements[playedNote];


	}
	return;
}


void setupPll()
{
	*PDRUNCFG &= ~(0x1 << 7); /*power up pll*/

	*SYSPLLCLKSEL &= ~0x3; /*set IRC as PLL input*/
	*SYSPLLCLKUEN = 0;
	*SYSPLLCLKUEN = 1;

	// Set Multiplication values M: 5, P: 2
	// resulting in a frequency of the current controlled osc of 12Mhz*M*2*P = 240MHz
	// output Frequency is 12MHz*5 = 60 MHz which is divided again to provide the system clock
	*SYSPLLCTRL = 0x4 | (0x1 << 5);

	while (*SYSPLLSTAT & 0x1 == 0)
	{}

	// set PLL output as main clock source
	*MAINCLKSEL = 0x3;
    *MAINCLKUEN = 0x0;
    *MAINCLKUEN = 0x1;

    // divide PLL clock by 2 to get 30 MHz
    *SYSAHBCLKDIV = 0x2;

}


int main(void) {


	// initialize cpu clock and basic peripheral clocks
	setupPll();
	*SYSAHBCLKCTRL |= ( (0x1 << SWM) | (0x1 << IOCON) );
	

	// enable  and reset used hardware components
	resetUart0();
	resetUart1();
	resetSCT();
	resetGPIO();
	resetTimer();


	// "driver" loading
	initTimer();

	
    initPwmDac();
	amplitude = 0xFF;

	//init Console Interface
	initConsoleUart();

	// init midi receiver
	initMidiUart();


	//init GPIO
	initGpio();

	
	// set channel 0 as a led blinker
	setDelay0(CLOCK_FREQ/1000);

	// set channel 1 as sample clock
	setDelay1((uint32_t)(CLOCK_FREQ/SAMPLING_FREQ));

	printf("LPC Synth ready\r\n");
    uint8_t pcnt;
    pcnt=0;
    char nr[4];

    uint32_t t_phase = 0;
    uint32_t t_sineval;
    uint32_t tic;
    uint32_t toc;
    char dbfr[11];

    //printf("computing sine vals\r\n");
    /*
    while (t_phase < 0xFFFF)
    {
    	setDelay2(0x7FFFFFFF);
    	tic=*MRT_TIMER2;
    	t_sineval = sineVal(t_phase);
    	toc=*MRT_TIMER2;
    	tic = tic - toc;
        for(uint8_t c=0;c<11;c++)
        {
        	*(dbfr + c) = 0;
        }
        printf("phase: ");

        asHex(t_phase,dbfr);
        printf(dbfr);
        for(uint8_t c=0;c<11;c++)
        {
        	*(dbfr + c) = 0;
        }
        printf(", val: ");
        asHex(t_sineval,dbfr);
        printf(dbfr);

        for(uint8_t c=0;c<11;c++)
        {
        	*(dbfr + c) = 0;
        }
        printf(", ticks: ");
        asHex(tic,dbfr);
        printf(dbfr);
        printf("\r\n");
        t_phase += 0x40;
    }
    */

    while(1) { // "OS"-Loop
    	//*NOT0 |= 0x1 << 2;
        //runTimer(1000);

    	//play_adagio_for_tron();
        //pcnt++;
    }
    return 0 ;
}
