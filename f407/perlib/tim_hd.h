#ifndef TIM_HD_H_SENTRY
#define TIM_HD_H_SENTRY

#include <stm32f4xx.h>
#include "general.h"

uint32_t tim6_get_ticks();
void tim6_set_ticks(const uint32_t ticks);
uint32_t tim7_get_ticks();
void tim7_set_ticks(const uint32_t ticks);

#endif
