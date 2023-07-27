#include "main.h"

void delay_ms(uint32_t ms);

int main(void)
{
	rcc_init();
	enable_irq();

	gpio_init(&led1_pin);
	systick_init(1000);
	
	while(1) {
		gpio_tgl(&led1_pin);	
		delay_ms(500);
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
