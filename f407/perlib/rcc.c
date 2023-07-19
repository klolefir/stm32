#include "rcc.h"

static void rcc_deinit()
{
	SET_BIT(RCC->CR, RCC_CR_HSION);
	while(READ_BIT(RCC->CR, RCC_CR_HSIRDY) == RESET) {}
	MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0X80U);
	CLEAR_REG(RCC->CFGR);
	while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET) {}
	CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
	while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET) {}
	CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
	while(READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET) {}
	CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);
	SET_BIT(RCC->CSR, RCC_CSR_RMVF);
	CLEAR_REG(RCC->CIR);

}

void rcc_init()
{
	rcc_deinit();
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;
	
	FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_PRFTEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;
	while(!(FLASH->ACR & FLASH_ACR_LATENCY_5WS)) {}
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY)) {}

	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLQ |
					  RCC_PLLCFGR_PLLM |
					  RCC_PLLCFGR_PLLN |
					  RCC_PLLCFGR_PLLP);

	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
	RCC->PLLCFGR |= (2 << RCC_PLLCFGR_PLLP_Pos);
	RCC->PLLCFGR |= (4 << RCC_PLLCFGR_PLLM_Pos);
	RCC->PLLCFGR |= (7 << RCC_PLLCFGR_PLLQ_Pos);
	RCC->PLLCFGR |= (168 << RCC_PLLCFGR_PLLN_Pos);

	//RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY)) {}

	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {}

	SystemCoreClockUpdate();
}

void rcc_enable(const periph_id_t periph_id)
{
	switch(periph_id) {
	case gpioa_id:	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
					break;

	case gpiob_id:	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;	
					break;

	case gpioc_id:	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
					break;

	case usart1_id:	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
					break;

	case usart2_id:	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
					break;

	case tim1_id:	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;	
					break;

	case spi1_id:	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;	
					break;

	default:		return;
	}
}

uint32_t rcc_get_ahb1_clk()
{
	return SystemCoreClock;
}

uint32_t rcc_get_apb1_clk()
{
	return (SystemCoreClock / rcc_apb1_div);
}

uint32_t rcc_get_apb2_clk()
{
	return (SystemCoreClock / rcc_apb2_div);
}
