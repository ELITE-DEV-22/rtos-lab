#include <stdint.h>
#include "stm32f103_regs.h"

#include "FreeRTOS.h"
#include "task.h"

typedef struct {
    GPIO_TypeDef *port;
    uint32_t pin;
    uint32_t period_ms;
} led_task_args_t;

static const led_task_args_t led_a = { GPIOA, 2, 1000 };
static const led_task_args_t led_b = { GPIOA, 0, 2000 };
static const led_task_args_t led_c = { GPIOA, 1, 3000 };

static void clock_setup_hse_pll_72mhz(void)
{
    RCC->CR |= RCC_CR_HSEON;

    for (volatile uint32_t timeout = 500000U; timeout > 0U; timeout--) {
        if ((RCC->CR & RCC_CR_HSERDY) != 0U) {
            break;
        }

        if (timeout == 1U) {
            return;
        }
    }

    FLASHREGS->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2WS;

    RCC->CFGR = RCC_CFGR_PPRE1_DIV2
              | RCC_CFGR_PLLSRC_HSE
              | RCC_CFGR_PLLXTPRE_DIV1
              | RCC_CFGR_PLLMUL_9;

    RCC->CR |= RCC_CR_PLLON;

    while ((RCC->CR & RCC_CR_PLLRDY) == 0U) {
    }

    RCC->CFGR = (RCC->CFGR & ~0x3U) | RCC_CFGR_SW_PLL;

    while ((RCC->CFGR & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_PLL) {
    }
}

static void gpio_output_init(GPIO_TypeDef *port, uint32_t pin)
{
    volatile uint32_t *cr = (pin < 8) ? &port->CRL : &port->CRH;
    uint32_t shift = (pin % 8U) * 4U;

    *cr &= ~(0xFU << shift);
    *cr |= (GPIO_CNF_MODE_OUTPUT_PP_2MHZ << shift);
}

static void led_task(void *pvParameters)
{
    const led_task_args_t *args = (const led_task_args_t *)pvParameters;

    for (;;) {
        args->port->ODR ^= (1U << args->pin);
        vTaskDelay(pdMS_TO_TICKS(args->period_ms));
    }
}

int main(void)
{
    clock_setup_hse_pll_72mhz();

    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;

    gpio_output_init(led_a.port, led_a.pin);
    gpio_output_init(led_b.port, led_b.pin);
    gpio_output_init(led_c.port, led_c.pin);

    xTaskCreate(led_task, "led_1s", configMINIMAL_STACK_SIZE,
                (void *)&led_a, tskIDLE_PRIORITY + 1, NULL);

    xTaskCreate(led_task, "led_2s", configMINIMAL_STACK_SIZE,
                (void *)&led_b, tskIDLE_PRIORITY + 1, NULL);

    xTaskCreate(led_task, "led_3s", configMINIMAL_STACK_SIZE,
                (void *)&led_c, tskIDLE_PRIORITY + 1, NULL);

    vTaskStartScheduler();

    for (;;) {
    }
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;

    for (;;) {
    }
}

void vApplicationIdleHook(void)
{
    __asm volatile ("wfi");
}

