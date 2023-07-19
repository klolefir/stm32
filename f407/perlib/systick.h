#ifndef SYSTICK_H_SENTRY
#define SYSTICK_H_SENTRY

#include "stm32f4xx.h"

enum {
	systick_reset_val = 1
};

void systick_init(const uint32_t freq);
void systick_set_freq(const uint32_t freq);
void systick_enable();
void systick_disable();
void systick_reset();

#endif
