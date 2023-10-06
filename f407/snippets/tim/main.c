#include "main.h"

int main(void)
{
	rcc_init();
	enable_irq();

	gpio_init(&led1_pin);
	tim_init(&tim6);
	
	while(1) {
	}
}

void tim6_dac_handler()
{
	uint8_t is_tim6 = read_bit(&TIM6->SR, TIM_SR_UIF);
	if(is_tim6) {
		clear_bit(&TIM6->SR, TIM_SR_UIF);
		gpio_tgl(&led1_pin);
	}
}
