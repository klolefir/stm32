#include "tim_hd.h"
#include "usart.h"

volatile uint32_t tim6_ticks = 0;
volatile uint32_t tim7_ticks = 0;

uint32_t tim6_get_ticks()
{
	return tim6_ticks;
}

void tim6_set_ticks(const uint32_t ticks)
{
	tim6_ticks = ticks;
}

uint32_t tim7_get_ticks()
{
	return tim7_ticks;
}

void tim7_set_ticks(const uint32_t ticks)
{
	tim7_ticks = ticks;
}

void tim6_dac_handler()
{
	TIM_TypeDef *tim = TIM6;
	uint8_t is_tim6 = read_bit(&tim->SR, TIM_SR_UIF);
	if(is_tim6) {
		uint32_t ticks = tim6_get_ticks();
		uint32_t new_ticks = ticks + 1;
		tim6_set_ticks(new_ticks);
		clear_bit(&tim->SR, TIM_SR_UIF);
	}
}

void tim7_handler()
{
	TIM_TypeDef *tim = TIM7;
	uint8_t is_tim7 = read_bit(&tim->SR, TIM_SR_UIF);
	if(is_tim7) {
		clear_bit(&tim->SR, TIM_SR_UIF);
		tim7_ticks++;
	}
}

