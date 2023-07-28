#include "main.h"

void delay_ms(uint32_t ms);

int main(void)
{
	rcc_init();
	enable_irq();

	//gpio_init(&led1_pin);
	systick_init(1000);

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODE6_0;
	//GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD6_Msk;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD6_0;
	
	while(1) {
	}
}

void delay_ms(uint32_t ms)
{
	uint32_t start = ticks;
	uint32_t end = start + ms;

	if(end < start)
		while(ticks > start) {}

	while(ticks < end) {}
}

void systick_handler()
{
	ticks++;
}
