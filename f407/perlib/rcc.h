#ifndef RCC_H_SENTRY
#define RCC_H_SENTRY

#include "stm32f4xx.h"

#if 0
typedef struct {
	ahb1_div;
	apb1_div;
	apb2_div;
} rcc_bus_div_t;

typedef struct {
	pllq;
	pllm;
	plln;
	pllp;
} rcc_pll_t;

typedef enum {
	rcc_source_hse,
	rcc_source_hsi,
	rcc_source_pll
} rcc_source_t

typedef enum {
	rcc_pll_source_hse,
	rcc_pll_source_hsi,
} rcc_pll_source_t
#else
typedef enum {
	rcc_ahb1_div = 1,
	rcc_apb1_div = 4,
	rcc_apb2_div = 2
} rcc_bus_div_t;
#endif

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
