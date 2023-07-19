#include "main.h"

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
	enable_irq();
	systick_init(1000);

	gpio_init(&led1_pin);
	usart_init(&usart1);

	while(1) {
		GPIOA->ODR ^= (1 << LED1_PIN);
		GPIOA->ODR ^= (1 << LED2_PIN);
	
		usart_put_str(&usart1, "im here\r");
		delay_ms(1000);
	}
}

void systick_handler()
{
	ticks++;
}

void usart1_handler()
{
	char c;
	usart_get_char(&usart1, &c);
	usart_put_char(&usart1, c);
}
