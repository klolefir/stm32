#ifndef NVIC_H_SENTRY
#define NVIC_H_SENTRY

#include "stm32f4xx.h"

typedef int8_t nvic_irq_prior_t ; 

enum {
	none_irq_prior = 0
};

typedef enum {
	none_irqn  	= 0,
	exti0_irqn  = EXTI0_IRQn,
	exti1_irqn	= EXTI1_IRQn,
	exti2_irqn	= EXTI2_IRQn,
	exti3_irqn 	= EXTI3_IRQn,
	exti4_irqn 	= EXTI4_IRQn,
	adc_irqn	= ADC_IRQn,
	tim2_irqn 	= TIM2_IRQn,
	tim3_irqn 	= TIM3_IRQn,
	tim6_irqn 	= TIM6_DAC_IRQn,
	tim7_irqn 	= TIM7_IRQn,
	usart1_irqn = USART1_IRQn,
	usart2_irqn = USART2_IRQn,
	spi1_irqn = SPI1_IRQn,
	spi2_irqn = SPI2_IRQn
} nvic_irqn_t;

void nvic_enable_irq(nvic_irqn_t irqn);
void nvic_disable_irq(nvic_irqn_t irqn);
void nvic_set_priority(nvic_irqn_t irqn, nvic_irq_prior_t priority);

#endif 
