#ifndef GENERAL_H_SENTRY
#define GENERAL_H_SENTRY

#include "stm32f4xx.h"

uint8_t get_bit(const uint32_t pos);
void set_bit(uint32_t *reg, const uint32_t bit);
void clear_bit(uint32_t *reg, const uint32_t bit);
void clear_reg(uint32_t *reg);

#endif
