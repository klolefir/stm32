#include <stdint.h>
#include <stm32f4xx.h>

#include "gpio.h"
#include "rcc.h"
#include "usart.h"

#define VECTOR_TAB_SRAM
#define LED1_PIN 6
#define LED2_PIN 7
void rcc_init();
void rcc_deinit();

volatile uint32_t ticks = 0;

void systick_handler()
{
	ticks++;
}

#if 0
void usart1_handler()
{
	while(!(USART1->SR & USART_SR_RXNE)) {}
	char c = USART1->DR;
	uart_put_char(USART1, c);
}
#endif
void delay_ms(uint32_t ms)
{
	uint32_t start = ticks;
	uint32_t end = start + ms;

	if(end < start)
		while(ticks > start) {}

	while(ticks < end) {}
}

int main(void)
{
	rcc_init();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
	__enable_irq();
	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_EnableIRQ(USART1_IRQn);

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	gpio_t led_pin = 	{ 
						.port = gpio_port_a, 
						.mask = 7, 
						.mode = gpio_mode_output, 
						.otype = gpio_otype_pp, 
						.ospeed = gpio_ospeed_low, 
						.pupd = gpio_pupd_pu, 
						.alt = gpio_alt_system 
						};
	usart_t usart1 = 	{ 
						.usart = USART1, 
						.baud = usart_baud_115200, 
						.rx_st = usart_rx_on,
						.tx_st = usart_tx_on,
						.tx_int_st = usart_tx_int_off,
						.rx_int_st = usart_rx_int_off
						};

	gpio_init(&led_pin);
	usart_init(&usart1);

	while(1) {
		GPIOA->ODR ^= (1 << LED1_PIN);
		GPIOA->ODR ^= (1 << LED2_PIN);
		usart_put_str(&usart1, "im here\r");
		delay_ms(1000);
	}
}


