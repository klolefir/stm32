#include "rcc.h"

void rcc_enable(periph_id_t periph_id)
{
	uint32_t bus, periph_en;
	switch(periph_id) {
	case gpioa_id:	bus 		= RCC->AHB1ENR;
					periph_en 	= RCC_AHB1ENR_GPIOAEN;
					break;

	case gpiob_id:	bus 		= RCC->AHB1ENR:
					periph_en	= RCC_AHB1ENR_GPIOBEN;
					break;

	case gpioc_id:	bus 		= RCC->AHB1ENR:
					periph_en	= RCC_AHB1ENR_GPIOCEN;
					break;

	case usart1_id:	bus 		= RCC->APB2ENR;
					periph_en 	= RCC_APB2ENR_USART1EN;
					break;

	case usart2_id:	bus 		= RCC->APB1ENR;
					periph_en 	= RCC_APB1ENR_USART2EN;
					break;

	case tim1_id:	bus 		= RCC->APB2ENR;
					periph_en 	= RCC_APB2ENR_TIM1EN;
					break;

	case spi1_id:	bus 		= RCC->APB2ENR;
					periph_en 	= RCC_APB2ENR_SPI1EN;
					break;

	default:		return;
	}

	bus |= periph_clk;
}
