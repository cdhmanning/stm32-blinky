#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define GPIO_BANK	GPIOC
#define GPIO_PIN	GPIO_Pin_13

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
        GPIO_SetBits(GPIO_BANK, GPIO_PIN);
    else
        GPIO_ResetBits(GPIO_BANK, GPIO_PIN);
    gpio_on = !gpio_on;
        
}

int main(void)
{

    GPIO_InitTypeDef gpio;
 
    
    SysTick_Config(72000000/1000);
    
    /* enable gpio clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
 
    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin = (GPIO_PIN);
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed =  GPIO_Speed_10MHz;
    GPIO_Init(GPIO_BANK, &gpio);
    
    __enable_irq();
 
 #if 0
    /* main program loop */
    for (;;) {
        /* set led on */
        GPIO_SetBits(GPIO_BANK, GPIO_PIN);
        /* delay */
        simple_delay(100000);
        /* clear led */
        GPIO_ResetBits(GPIO_BANK, GPIO_PIN);
        /* delay */
        simple_delay(100000);
    }
 
 #else
     while(1) {
         __WFI();
    }
 #endif
 
    /* never reached */
    return 0;
}
