#include "nvic.h"

void nvic_enable_irq(irqn_t irqn)
{
	NVIC_EnableIRQ(irqn);
}

void nvic_disable_irq(irqn_t irqn)
{
	NVIC_DisableIRQ(irqn);
}

void nvic_set_priority(irqn_t irqn, int8_t priority)
{
	NVIC_SetPriority(irqn, priority);
}
