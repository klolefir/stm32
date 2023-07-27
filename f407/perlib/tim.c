#include "tim.h"
#include "nvic.h"
#include "rcc.h"

void tim_init(const tim_t *tim_setup)
{
	TIM_TypeDef *tim;
	rcc_periph_id_t tim_id;
	nvic_irqn_t tim_irqn;
	nvic_irq_prior_t tim_irq_priority;

	tim_num_t tim_num = tim_setup->tim_num;
	switch(tim_num) {
	case tim_6:	tim = TIM6;
				tim_id = tim6_id;
				tim_irqn = tim6_irqn;
				tim_irq_priority = tim6_irq_prior;
				break;
	case tim_7: tim = TIM7;
				tim_id = tim7_id;
				tim_irqn = tim7_irqn;
				tim_irq_priority = tim7_irq_prior;
				break;
	}
	rcc_enable(tim_id);

	tim_irq_state_t irq_st = tim_setup->irq_st;
	if(irq_st == tim_irq_on) {
		nvic_enable_irq(tim_irqn);
		nvic_set_priority(tim_irqn, tim_irq_priority);
	//	tim->SR &= ~TIM_SR_UIF;		//reset interrupt status
	//	tim->DIER |= TIM_DIER_UIE;	//enable interrupt
		clear_bit(&tim->SR, TIM_SR_UIF);
		set_bit(&tim->DIER, TIM_DIER_UIE);
	}


	tim_psc_t psc = tim_setup->psc;
	psc = psc - 1;
	//tim->PSC = psc - 1;
	set_reg(&tim->PSC, psc);

	tim_arr_t arr = tim_setup->arr;
	arr = arr - 1;
	set_reg(&tim->ARR, arr);
	//tim->ARR = arr - 1;

	tim_power_state_t pwr_st = tim_setup->pwr_st;
	set_bit(&tim->CR1, (pwr_st << TIM_CR1_CEN_Pos));
	//tim->CR1 |= (pwr_st << TIM_CR1_CEN_Pos);
}


void tim_enable(tim_t *tim_setup)
{

}

void tim_disable(tim_t *tim_setup)
{

}

void tim_enable_irq(tim_t *tim_setup)
{

	TIM_TypeDef *tim;
	nvic_irqn_t tim_irqn;
	nvic_irq_prior_t tim_irq_priority;

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
	}

	tim_setup->irq_st = tim_irq_on;
	nvic_enable_irq(tim_irqn);
	nvic_set_priority(tim_irqn, tim_irq_priority);
	//tim->SR &= ~TIM_SR_UIF;		//reset interrupt status
	clear_bit(&tim->SR, TIM_SR_UIF);
	//tim->DIER |= TIM_DIER_UIE;	//enable interrupt
	set_bit(&tim->DIER, TIM_DIER_UIE);
}

void tim_disable_irq(tim_t *tim_setup)
{
	TIM_TypeDef *tim;
	nvic_irqn_t tim_irqn;

	tim_num_t tim_num = tim_setup->tim_num;
	switch(tim_num) {
	case tim_6:	tim = TIM6;
				tim_irqn = tim6_irqn;
				break;
	case tim_7: tim = TIM7;
				tim_irqn = tim7_irqn;
				break;
	}

	tim_setup->irq_st = tim_irq_off;
	nvic_disable_irq(tim_irqn);
	tim->DIER &= ~TIM_DIER_UIE;	//disable interrupt
}
