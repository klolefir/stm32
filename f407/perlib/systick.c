#include "systick.h"

static volatile uint32_t ticks = 0;

void systick_init(const uint32_t freq)
{
	systick_set_freq(freq);
	systick_reset();
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	systick_enable();
}

void systick_handler()
{
	uint32_t ticks = systick_get_ticks();
	uint32_t new_ticks = ticks + 1;
	systick_set_ticks(new_ticks);
}

void systick_delay_ms(const uint32_t ms)
{
	uint32_t start = ticks;
	uint32_t end = start + ms;

	if(end < start)
		while(ticks > start) {}

	while(ticks < end) {}
}

uint32_t systick_get_ticks()
{
	return ticks;
}

void systick_set_ticks(const uint32_t new_ticks)
{
	ticks = new_ticks;
}


void systick_enable()
{
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void systick_disable()
{
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void systick_set_freq(const uint32_t freq)
{
	SysTick->LOAD = SystemCoreClock / freq;
}

void systick_reset()
{
	SysTick->VAL = systick_reset_val;
}
