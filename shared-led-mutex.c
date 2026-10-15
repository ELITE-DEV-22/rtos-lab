#include <stdint.h>
#include "stm32f103_regs.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

static SemaphoreHandle_t xLedMutex;

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
        xSemaphoreTake(xLedMutex, portMAX_DELAY);
        GPIOC->ODR ^= (1U << 13);           /* on */
        vTaskDelay(pdMS_TO_TICKS(1000));
        GPIOC->ODR ^= (1U << 13);           /* off */
        vTaskDelay(pdMS_TO_TICKS(1000));
        xSemaphoreGive(xLedMutex);
        /* the mutex is held for the WHOLE 2 s blink, not just each toggle */
    }
}

static void task2_toggle_2hz(void *pvParameters)
{
    (void)pvParameters;
    for (;;) {
        xSemaphoreTake(xLedMutex, portMAX_DELAY);
        GPIOC->ODR ^= (1U << 1);           /* on */
        vTaskDelay(pdMS_TO_TICKS(500));
        GPIOC->ODR ^= (1U << 1);           /* off */
        vTaskDelay(pdMS_TO_TICKS(500));
        xSemaphoreGive(xLedMutex);
        /* held for the WHOLE 1 s blink, not just each toggle */
    }
}

int main(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN;
    gpio_output_init(GPIOC, 13);
    gpio_output_init(GPIOA, 1);

    xLedMutex = xSemaphoreCreateMutex();

    xTaskCreate(task1_toggle_1hz, "task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(task2_toggle_2hz, "task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

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