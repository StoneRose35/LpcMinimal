
#ifndef __CHIP_H_
#define __CHIP_H_


#include <stdint.h>
#include <stdbool.h>


/** minimal register and constant definitions
 *
 *
 */

// constants
#define MRT_STAT_INTFLAG         (0x01)
#define SWM (7)
#define IOCON (18)
#define CLOCK_FREQ (12000000)
#define MRT_IRQN 10

// individual registers 
#define DIR0  ((volatile uint32_t*)0xA0002000UL) /*GPIO Direction Register*/
#define PORT2 ((volatile uint32_t*)0xA0000002UL) /*bit-register for GPIO Port*/
#define SET0 ((volatile uint32_t*)0xA0002200UL) /*register for setting all GPIO Pins at once*/
#define CLR0 ((volatile uint32_t*) 0xA0002280UL) /*register for clearing all GPIO Pins at once*/
#define PINENABLE0 ((volatile uint32_t*)0x4000C1C0UL) /*Part of the switch matrix which defines the default function fo the pins*/
#define PRESETCTRL ((volatile uint32_t*)0x40048004UL) /* register used for resetting the peripheral components used*/
#define SYSAHBCLKCTRL ((volatile uint32_t*)0x40048080UL) /*defines the routing of the system clock to the peripheral component, the clock must be enabled for all peripherals used*/
#define MRT_INTVAL0 ((volatile uint32_t*)0x40004000UL) /*Interrupt register for the Multi Rate Timer Channel 0 */
#define MRT_CTRL0 ((volatile uint32_t*)0x40004008UL) /*Control register for the Multi Rate Timer Channel 0 */
#define MRT_STAT0 ((volatile uint32_t*)0x4000400CUL) /*Status register for the Multi Rate Timer Channel 0 */
#define NVIC_ISER0 ((volatile uint32_t*)0xE000E100UL) /*Nested Vector Interrupt Controller register for enabling interrupts*/




#endif /* __CHIP_H_ */
