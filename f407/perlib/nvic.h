#ifndef NVIC_H_SENTRY
#define NVIC_H_SENTRY

#include "stm32f4xx.h"

typedef enum {
	exti0_irqn  = EXTI0_IRQn,
	exti1_irqn	= EXTI1_IRQn,
	exti2_irqn	= EXTI2_IRQn,
	exti3_irqn 	= EXTI3_IRQn,
	exti4_irqn 	= EXTI4_IRQn,
	adc_irqn	= ADC_IRQn,
	tim2_irqn 	= TIM2_IRQn,
	tim3_irqn 	= TIM3_IRQn,
	usart1_irqn = USART1_IRQn,
	usart2_irqn = USART2_IRQn
} irqn_t;

void nvic_enable_irq(irqn_t irqn);
void nvic_disable_irq(irqn_t irqn);
void nvic_set_priority(irqn_t irqn, int8_t priority);

#endif 
