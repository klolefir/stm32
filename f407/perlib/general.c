#include "general.h"

uint8_t get_bit(const uint32_t pos)
{
	return (1 << pos);
}

void set_bit(volatile uint32_t *reg, const uint32_t bit)
{
	*reg |= bit;
}

void clear_bit(volatile uint32_t *reg, const uint32_t bit)
{
	*reg &= ~bit;
}

uint8_t read_bit(const volatile uint32_t *reg, const uint32_t bit)
{
	uint8_t val = ((*reg) & bit);
	return val;
}

void set_reg(volatile uint32_t *reg, const uint32_t val)
{
	*reg = val;
}

void clear_reg(volatile uint32_t *reg)
{
	*reg = 0;
}
