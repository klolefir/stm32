#ifndef GENERAL_H_SENTRY
#define GENERAL_H_SENTRY

#include "stm32f4xx.h"

uint8_t get_bit(const uint32_t pos);
void set_bit(volatile uint32_t *reg, const uint32_t bit);
void clear_bit(volatile uint32_t *reg, const uint32_t bit);
uint8_t read_bit(const volatile uint32_t *reg, const uint32_t bit);
void set_reg(volatile uint32_t *reg, const uint32_t val);
void clear_reg(volatile uint32_t *reg);

#endif
