// cd stm32f103
// git clone --depth 1 https://github.com/FreeRTOS/FreeRTOS-Kernel.git FreeRTOS

// cd led-blink            # or three-thread-blink
// make                     # -> build/<project>.elf/.bin/.hex
// make flash               # st-flash write build/<project>.bin 0x8000000
// make size
// make clean

#include <stdint.h>
#include "stm32f103_regs.h"
static volatile uint32_t g_mstick;

void SysTick_Handler(void){
    g_msticks++;
}

static void delay_ms(uint_t ms){
    uint32_t target = g_msticks + ms;
    while (g_msticks < target){

    }
}

int main(void){
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; // enable GPIO clock
    
    /* PC13: output push-pull, 2 MHz (pin 13 -> CRH, nibble at bit 20) */
    GPIOC->CRH &= ~(0XFU<<20);
    GPIO->CRH |= (GPIO_CNF_MODE_OUTPUT_PP_2MHZ << 20);

    SYSTICK->LOAD = (8000000U / 1000U) - 1U;
    SYSTICK->VAL = 0;
    SYSTICK->CTRL = SYSTICK_CTRL_CLKSOURCE | SYSTICK_CTRL_TICKINT | SYSTICK_CTRL_ENABLE;

    for(;;){
    GPIO->ODR ^= (1U << 13);
    delay_ms(500)
    }
}