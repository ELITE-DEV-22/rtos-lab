#include <stdint.h>
#include "stm32f103_regs.h"

#include "FreeRTOS.h"
#include "task.h"

static void gpio_output_init(GPIO_TypeDef *port, uint32_t pin)
{
    volatile uint32_t *cr = (pin < 8) ? &port->CRL : &port->CRH;
    uint32_t shift = (pin % 8U) * 4U;

    *cr &= ~(0xFU << shift);
    *cr |=  (GPIO_CNF_MODE_OUTPUT_PP_2MHZ << shift);
}

static void task1_toggle_1hz(void *pvParameters)
{
    (void)pvParameters;
    for (;;) {
        GPIOA->ODR ^= (1U << 1);           /* on */
        vTaskDelay(pdMS_TO_TICKS(1000));
        GPIOA->ODR ^= (1U << 1);           /* off */
        vTaskDelay(pdMS_TO_TICKS(1000));
        /* one full loop iteration = one complete blink, 2000 ms total */
    }
}

static void task2_toggle_2hz(void *pvParameters)
{
    (void)pvParameters;
    for (;;) {
        GPIOA->ODR ^= (1U << 1);           /* on */
        vTaskDelay(pdMS_TO_TICKS(500));
        GPIOA->ODR ^= (1U << 1);           /* off */
        vTaskDelay(pdMS_TO_TICKS(500));
        /* one full loop iteration = one complete blink, 1000 ms total */
    }
}

int main(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    gpio_output_init(GPIOA, 1);
    // gpio_output_init(GPIOA, 1);

    xTaskCreate(task1_toggle_1hz, "task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    // xTaskCreate(task2_toggle_2hz, "task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    vTaskStartScheduler();

    for (;;) {
        /* only reached if vTaskStartScheduler() fails, e.g. out of heap */
    }
}

/* Required because FreeRTOSConfig.h sets configCHECK_FOR_STACK_OVERFLOW=2 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;
    for (;;) {
    }
}