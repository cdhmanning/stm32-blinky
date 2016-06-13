/*
* LED connected to PB0
* do not forget current limiting resistor(!)
*/

#define STM32F10X_MD
#include "stm32f10x.h"

// RAM starts at 0x20000000
// Stack at 0x20001000 (4Kbyte stack)
__asm__(".word 0x20001000");
__asm__(".word main");

int  main()
{
	unsigned int c = 0;

	// enable port B clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

	// set up pin B0 as output (push-pull)
	GPIOB->CRL &= 0xfffffff0;
	GPIOB->CRL |= 0x00000001;

	while(1) {
		GPIOB->BSRR = (1 << 0); // ON
		for(c = 0; c < 100000; c++);
		GPIOB->BSRR = (1 << 16); // OFF
		for(c = 0; c < 400000; c++);
	}
}
