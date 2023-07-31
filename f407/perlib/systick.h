#ifndef SYSTICK_H_SENTRY
#define SYSTICK_H_SENTRY

#include "stm32f4xx.h"

enum {
	systick_reset_val = 0
};

void systick_init(const uint32_t freq);
void systick_set_freq(const uint32_t freq);
void systick_delay_ms(const uint32_t ms);
void systick_enable();
void systick_disable();
void systick_reset();
uint32_t systick_get_ticks();
void systick_set_ticks(const uint32_t new_ticks);

#endif
