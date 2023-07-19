#include "general.h"

uint8_t get_bit(const uint32_t pos)
{
	return (1 << pos);
}

void set_bit(uint32_t *reg, const uint32_t bit)
{
	*reg |= bit;
}

void clear_bit(uint32_t *reg, const uint32_t bit)
{
	*reg &= ~bit;
}

void clear_reg(uint32_t *reg)
{
	*reg = 0;
}
