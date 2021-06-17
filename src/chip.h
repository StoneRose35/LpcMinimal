
#ifndef __CHIP_H_
#define __CHIP_H_


/** minimal register and constant definitions
 *
 *
 */

// constants
#define MRT_STAT_INTFLAG         (0x01)
#define SWM (7)
#define IOCON (18)
#define CLOCK_FREQ (30000000)
#define MRT_IRQN 10

#define UART_CLOCK_DIV CLOCK_FREQ/31250/16
#define RXRDY 1

#define SCT_UNIFY 0

#define SYSPLL_PD 7


// individual registers 
#define DIR0  ((volatile uint32_t*)0xA0002000UL) /*GPIO Direction Register*/
#define PORT2 ((volatile uint32_t*)0xA0000002UL) /*bit-register for GPIO Port*/
#define SET0 ((volatile uint32_t*)0xA0002200UL) /*register for setting all GPIO Pins at once*/
#define CLR0 ((volatile uint32_t*)0xA0002280UL) /*register for clearing all GPIO Pins at once*/
#define NOT0 ((volatile uint32_t*)0xA0002300UL) /*register for toggling the gpio pins*/
#define PINENABLE0 ((volatile uint32_t*)0x4000C1C0UL) /*Part of the switch matrix which defines the default function fo the pins*/
#define PRESETCTRL ((volatile uint32_t*)0x40048004UL) /* register used for resetting the peripheral components used*/
#define SYSAHBCLKDIV ((volatile uint32_t*)0x40048078UL) /*Systm clock divider register*/
#define SYSAHBCLKCTRL ((volatile uint32_t*)0x40048080UL) /*defines the routing of the system clock to the peripheral component, the clock must be enabled for all peripherals used*/
#define MRT_INTVAL0 ((volatile uint32_t*)0x40004000UL) /*Interrupt register for the Multi Rate Timer Channel 0 */
#define MRT_CTRL0 ((volatile uint32_t*)0x40004008UL) /*Control register for the Multi Rate Timer Channel 0 */
#define MRT_STAT0 ((volatile uint32_t*)0x4000400CUL) /*Status register for the Multi Rate Timer Channel 0 */
#define MRT_INTVAL1 ((volatile uint32_t*)0x40004010UL) /*Interrupt register for the Multi Rate Timer Channel 1 */
#define MRT_CTRL1 ((volatile uint32_t*)0x40004018UL) /*Control register for the Multi Rate Timer Channel 1 */
#define MRT_STAT1 ((volatile uint32_t*)0x4000401CUL) /*Status register for the Multi Rate Timer Channel 1 */

#define NVIC_ISER0 ((volatile uint32_t*)0xE000E100UL) /*Nested Vector Interrupt Controller register for enabling interrupts*/

#define PINASSIGN6  ((volatile uint32_t*)0x4000C018UL) /*switch matrix used to setup pwm output pin*/
#define PINASSIGN7 ((volatile uint32_t*)0x4000C01CUL) /*switch matrix used to setup pwm output pin*/
#define SCT_CONFIG ((volatile uint32_t*)0x50004000UL) /*SCT config register*/
#define SCT_COUNT  ((volatile uint32_t*)0x50004040UL) /*SCT counter value*/

#define SCT_MATCH0_ALT ((BARE_REGISTER_T*)0x50004100UL)
#define SCT_MATCH0 ((SCT_MATCH_T*)0x50004100UL) /*SCT match 0  register, hold the PWM value*/
#define SCT_MATCHREL0 ((SCT_MATCH_T*)0x50004200UL) /*SCT match 0 reload register, used to update the pwm value*/
#define SCT_MATCH1 ((SCT_MATCH_T*)0x50004104UL) /*SCT match 0  register, defines the maximum counter resolution / PWM frequency*/
#define SCT_MATCHREL1 ((SCT_MATCH_T*)0x50004204UL)
#define SCT_EV0_CTRL ((volatile uint32_t*)0x50004304UL) /*SCT Event 0 control*/
#define SCT_EV1_CTRL ((volatile uint32_t*)0x50004308UL) /*SCT Event 1 control*/
#define SCT_CTRL ((volatile uint32_t*)0x50004004UL) /*SCT Control Register*/
#define SCT_LIMIT ((volatile uint32_t*)0x50004008UL) /* SCT Limit Register*/
#define SCT_OUT0_SET ((volatile uint32_t*)0x50004500UL) /*SCT Output 0 Set on Event*/
#define SCT_OUT0_CLR ((volatile uint32_t*)0x50004504UL) /*SCT Output 0 Clear on Events*/
#define SCT_OUTPUT  ((volatile uint32_t*)0x50004050UL) /*SCT Output 0 register*/
#define EV0_STATE ((volatile uint32_t*)0x50004300UL) /*Event 0 State register*/
#define EV1_STATE ((volatile uint32_t*)0x50004308UL) /*Event 1 State register*/


#define INTENSET ((volatile uint32_t*)0x4006400CUL) /*USART Interrupt set Register*/
#define PINASSIGN0 ((volatile uint32_t*)0x4000C018UL) /*switch matrix assign register to set midi in*/
#define UARTCLKDIV ((volatile uint32_t*)0x40048094UL) /*UART Clock Divider register*/
#define UARTFRGMULT ((volatile uint32_t*)0x400480F4UL) /*UART Fraction clock divider  multiplicator*/
#define UARTFRGDIV ((volatile uint32_t*)0x400480F0UL) /*UART Fraction clock divider  divisor*/
#define BRG0 ((volatile uint32_t*)0x40064020UL) /*baud rate generation divider*/
#define UARTCFG0 ((volatile uint32_t*)0x40064000UL) /*UART0 Configuration register*/
#define USART_STAT0 ((volatile uint32_t*)0x40064008UL) /*UART0 Status register*/
#define RXDAT0 ((volatile uint32_t*)0x40064014UL) /*USART0 Receiver Data Buffer*/

#define PDRUNCFG ((volatile uint32_t*)0x40048238UL) /*Power configuration register*/
#define SYSPLLCLKSEL ((volatile uint32_t*)0x40048040UL) /*PLL Clock selection*/
#define SYSPLLCLKUEN ((volatile uint32_t*)0x40048044UL) /*PLL Clock update register*/
#define SYSPLLCTRL ((volatile uint32_t*)0x40048008UL) /*PLL Control register*/
#define SYSPLLSTAT ((volatile uint32_t*)0x4004800CUL) /*PLL Lock status*/

#define MAINCLKSEL ((volatile uint32_t*)0x40048070UL) /*Main clock source selection */
#define MAINCLKUEN ((volatile uint32_t*)0x40048074UL) /*Main Clock update register*/


typedef struct {
  volatile union {
	  uint32_t REG_VAL;
	  struct {
		  uint8_t VAL;
		  uint8_t RES0;
		  uint8_t RES1;
		  uint8_t RES2;
	  };
 };
} SCT_MATCH_T;


typedef struct
{
  volatile uint32_t VAL;
} BARE_REGISTER_T;




#endif /* __CHIP_H_ */


