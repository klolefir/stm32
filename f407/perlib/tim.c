#include "tim.h"
#include "tim_hd.h"
#include "nvic.h"
#include "rcc.h"

#include <stddef.h>

static TIM_TypeDef *switch_tim(const tim_t *tim_setup);
static nvic_irqn_t switch_irqn(const tim_t *tim_setup);
static nvic_irq_prior_t switch_irq_prior(const tim_t *tim_setup);
static rcc_periph_id_t switch_tim_id(const tim_t *tim_setup);

void tim_init(const tim_t *tim_setup)
{
	TIM_TypeDef *tim = switch_tim(tim_setup);
	rcc_periph_id_t tim_id = switch_tim_id(tim_setup);
	nvic_irqn_t tim_irqn = switch_irqn(tim_setup);
	nvic_irq_prior_t tim_irq_prior = switch_irq_prior(tim_setup);

	rcc_enable(tim_id);

	tim_irq_state_t irq_st = tim_setup->irq_st;
	if(irq_st == tim_irq_on) {
		nvic_enable_irq(tim_irqn);
		nvic_set_priority(tim_irqn, tim_irq_prior);
		clear_bit(&tim->SR, TIM_SR_UIF);	//reset interrupt status
		set_bit(&tim->DIER, TIM_DIER_UIE);	//enable interrupt
	}

	tim_psc_t psc = tim_setup->psc;
	psc = psc - 1;
	set_reg(&tim->PSC, psc);

	tim_arr_t arr = tim_setup->arr;
	arr = arr - 1;
	set_reg(&tim->ARR, arr);

	tim_power_state_t pwr_st = tim_setup->pwr_st;
	set_bit(&tim->CR1, (pwr_st << TIM_CR1_CEN_Pos));
}


void tim_enable(tim_t *tim_setup)
{
	TIM_TypeDef *tim = switch_tim(tim_setup);
	set_bit(&tim->CR1, TIM_CR1_CEN);
}

void tim_disable(tim_t *tim_setup)
{
	TIM_TypeDef *tim = switch_tim(tim_setup);
	clear_bit(&tim->CR1, TIM_CR1_CEN);
}

void tim_enable_irq(tim_t *tim_setup)
{
	TIM_TypeDef *tim = switch_tim(tim_setup);
	nvic_irqn_t tim_irqn = switch_irqn(tim_setup);
	nvic_irq_prior_t tim_irq_prior = switch_irq_prior(tim_setup);

#if 0
	tim_num_t tim_num = tim_setup->tim_num;
	switch(tim_num) {
	case tim_6:	tim = TIM6;
				tim_irqn = tim6_irqn;
				tim_irq_priority = tim6_irq_prior;
				break;

	case tim_7: tim = TIM7;
				tim_irqn = tim7_irqn;
				tim_irq_priority = tim7_irq_prior;
				break;

	default:	return;
	}
#endif

	tim_setup->irq_st = tim_irq_on;
	nvic_enable_irq(tim_irqn);
	nvic_set_priority(tim_irqn, tim_irq_prior);
	clear_bit(&tim->SR, TIM_SR_UIF);
	set_bit(&tim->DIER, TIM_DIER_UIE);
}

void tim_disable_irq(tim_t *tim_setup)
{
	TIM_TypeDef *tim = switch_tim(tim_setup);
	nvic_irqn_t tim_irqn = switch_irqn(tim_setup);

	tim_setup->irq_st = tim_irq_off;
	nvic_disable_irq(tim_irqn);
	tim->DIER &= ~TIM_DIER_UIE;	//disable interrupt
}

uint32_t tim_get_ticks(const tim_t *tim_setup)
{
	uint32_t ticks;
	tim_num_t tim_num = tim_setup->tim_num;

	switch(tim_num) {
	case tim_6:	ticks = tim6_get_ticks();
	case tim_7:	ticks = tim7_get_ticks();
	default:	ticks = 0;
	}

	return ticks;
}

void tim_set_ticks(const tim_t *tim_setup, const uint32_t ticks)
{
	tim_num_t tim_num = tim_setup->tim_num;

	switch(tim_num) {
	case tim_6:	tim6_set_ticks(ticks);
	case tim_7:	tim7_set_ticks(ticks);
	default:	return;
	}
}

TIM_TypeDef *switch_tim(const tim_t *tim_setup)
{
	tim_num_t tim_num = tim_setup->tim_num;
	switch(tim_num) {
	case tim_6:	return TIM6;
	case tim_7: return TIM7;
	default:	return NULL;
	}
}

nvic_irqn_t switch_irqn(const tim_t *tim_setup)
{
	tim_num_t tim_num = tim_setup->tim_num;
	switch(tim_num) {
	case tim_6:	return tim6_irqn;
	case tim_7: return tim7_irqn;
	default:	return none_irqn;
	}
}

nvic_irq_prior_t switch_irq_prior(const tim_t *tim_setup)
{
	tim_num_t tim_num = tim_setup->tim_num;
	switch(tim_num) {
	case tim_6:	return tim6_irq_prior;	
	case tim_7: return tim7_irq_prior;
	default:	return none_irq_prior;
	}
}

rcc_periph_id_t switch_tim_id(const tim_t *tim_setup)
{
	tim_num_t tim_num = tim_setup->tim_num;
	switch(tim_num) {
	case tim_6:	return tim6_id;
	case tim_7: return tim7_id;
	default:	return none_id;
	}
}
