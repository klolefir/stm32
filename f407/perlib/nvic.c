#include "nvic.h"

void nvic_enable_irq(nvic_irqn_t irqn)
{
	NVIC_EnableIRQ(irqn);
}

void nvic_disable_irq(nvic_irqn_t irqn)
{
	NVIC_DisableIRQ(irqn);
}

void nvic_set_priority(nvic_irqn_t irqn, nvic_irq_prior_t priority)
{
	NVIC_SetPriority(irqn, priority);
}
