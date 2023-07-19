#include "systick.h"

void systick_init(const uint32_t freq)
{
	systick_set_freq(freq);
	systick_reset();
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	systick_enable();
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
