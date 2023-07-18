#ifndef RCC_H_SENTRY
#define RCC_H_SENTRY

#include "stm32f4xx.h"

typedef enum {
	rcc_apb1_div = 4,
	rcc_apb2_div = 2
} rcc_bus_div_t;

typedef enum {
	gpioa_id,
	gpiob_id,
	gpioc_id,
	gpiod_id,
	usart1_id,
	usart2_id,
	spi1_id,
	spi2_id,
	tim1_id,
	tim2_id
} periph_id_t;

void rcc_init();
void rcc_enable(periph_id_t periph_id);

#endif
