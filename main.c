#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define GPIO_BANK	GPIOC
#define GPIO_PIN13	GPIO_Pin_13
#define GPIO_PIN14	GPIO_Pin_14


/* timing is not guaranteed :) */
void simple_delay(uint32_t us)
{
        us *= 10;
        
	/* simple delay loop */
	while (us--) {
		asm volatile ("nop");
	}
}



void  SysTick_Handler(void)
{
    static uint32_t gpio_on;
    static uint32_t flash_count;
    
    flash_count++;
    
    if (flash_count < 500)
        return;
        
    flash_count = 0;
    
    if (gpio_on)
        GPIO_SetBits(GPIO_BANK, GPIO_PIN13);
    else
        GPIO_ResetBits(GPIO_BANK, GPIO_PIN13);
    gpio_on = !gpio_on;
        
}

int main(void)
{
    GPIO_InitTypeDef gpio;
 
    /* enable gpio clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
 
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin = (GPIO_PIN13);
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed =  GPIO_Speed_50MHz;
    GPIO_Init(GPIO_BANK, &gpio);

    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin = (GPIO_PIN14);
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed =  GPIO_Speed_50MHz;
    GPIO_Init(GPIO_BANK, &gpio);
 
 #if 1
    /* main program loop */
    while(1) {
        /* Make 2 sqaure waves 90 degrees out of phase. */

#if 0
        GPIO_BANK->BSRR = GPIO_PIN13;
        GPIO_BANK->BSRR = GPIO_PIN13;
        GPIO_BANK->BSRR = GPIO_PIN13;
        GPIO_BANK->BSRR = GPIO_PIN14;
        GPIO_BANK->BSRR = GPIO_PIN14;
        GPIO_BANK->BSRR = GPIO_PIN14;
        GPIO_BANK->BRR = GPIO_PIN13;
        GPIO_BANK->BRR = GPIO_PIN13;
        GPIO_BANK->BRR = GPIO_PIN13;
        GPIO_BANK->BRR = GPIO_PIN14;
        GPIO_BANK->BRR = GPIO_PIN14;
        GPIO_BANK->BRR = GPIO_PIN14;
#else
        GPIO_BANK->BSRR = GPIO_PIN13;
        GPIO_BANK->BSRR = GPIO_PIN13;
        GPIO_BANK->BSRR = GPIO_PIN14;
        GPIO_BANK->BSRR = GPIO_PIN14;
        GPIO_BANK->BRR = GPIO_PIN13;
        GPIO_BANK->BRR = GPIO_PIN13;
        GPIO_BANK->BRR = GPIO_PIN14;
        GPIO_BANK->BRR = GPIO_PIN14;

        GPIO_BANK->BSRR = GPIO_PIN13;
        GPIO_BANK->BSRR = GPIO_PIN13;
        GPIO_BANK->BSRR = GPIO_PIN14;
        GPIO_BANK->BSRR = GPIO_PIN14;
        GPIO_BANK->BRR = GPIO_PIN13;
        GPIO_BANK->BRR = GPIO_PIN13;
        GPIO_BANK->BRR = GPIO_PIN14;
        GPIO_BANK->BRR = GPIO_PIN14;

        GPIO_BANK->BSRR = GPIO_PIN13;
        GPIO_BANK->BSRR = GPIO_PIN13;
        GPIO_BANK->BSRR = GPIO_PIN14;
        GPIO_BANK->BSRR = GPIO_PIN14;
        GPIO_BANK->BRR = GPIO_PIN13;
        GPIO_BANK->BRR = GPIO_PIN13;
        GPIO_BANK->BRR = GPIO_PIN14;
        GPIO_BANK->BRR = GPIO_PIN14;

        GPIO_BANK->BSRR = GPIO_PIN13;
        GPIO_BANK->BSRR = GPIO_PIN13;
        GPIO_BANK->BSRR = GPIO_PIN14;
        GPIO_BANK->BSRR = GPIO_PIN14;
        GPIO_BANK->BRR = GPIO_PIN13;
        GPIO_BANK->BRR = GPIO_PIN13;
        GPIO_BANK->BRR = GPIO_PIN14;
        GPIO_BANK->BRR = GPIO_PIN14;

        GPIO_BANK->BSRR = GPIO_PIN13;
        GPIO_BANK->BSRR = GPIO_PIN13;
//        GPIO_BANK->BSRR = GPIO_PIN14;
//        GPIO_BANK->BSRR = GPIO_PIN14;
        GPIO_BANK->BRR = GPIO_PIN13;
        GPIO_BANK->BRR = GPIO_PIN13;
        GPIO_BANK->BRR = GPIO_PIN14;
        GPIO_BANK->BRR = GPIO_PIN14;
#endif
        /* delay */
        //simple_delay(100000);
        /* clear led */
        //GPIO_ResetBits(GPIO_BANK, GPIO_PIN);
        /* delay */
        //simple_delay(100000);
    }
 
 #else
     SysTick_Config(72000000/1000);
    
     __enable_irq();

     while(1) {
         __WFI();
    }
 #endif
 
    /* never reached */
    return 0;
}
