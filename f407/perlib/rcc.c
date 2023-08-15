#include "rcc.h"

static volatile rcc_clk_t system_clk;

void rcc_deinit()
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

#if 1
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;
#endif

	SET_BIT(RCC->CR, RCC_CR_HSEON);
	while(READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET) {}

	/*FLASH->ACR &= ~(FLASH_ACR_ICEN | FLASH_ACR_PRFTEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY);*/
	FLASH->ACR &= ~(FLASH_ACR_ICEN | FLASH_ACR_PRFTEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY);
	/*FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_PRFTEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;*/
	FLASH->ACR |= FLASH_ACR_LATENCY_5WS;
#if 0
	CLEAR_BIT(FLASH->ACR, FLASH_ACR_PRFTEN);
	SET_BIT(FLASH->ACR, FLASH_ACR_PRFTEN);
	MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_5WS);
#endif

	MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV4);
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV2);

	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLQ |
					  RCC_PLLCFGR_PLLM |
					  RCC_PLLCFGR_PLLN |
					  RCC_PLLCFGR_PLLP |
					  RCC_PLLCFGR_PLLSRC);

#if 1
	RCC->PLLCFGR |= (4 << RCC_PLLCFGR_PLLM_Pos);
	RCC->PLLCFGR |= (168 << RCC_PLLCFGR_PLLN_Pos);
	RCC->PLLCFGR |= (0 << RCC_PLLCFGR_PLLP_Pos);
	RCC->PLLCFGR |= (7 << RCC_PLLCFGR_PLLQ_Pos);
#else
	RCC->PLLCFGR |= (4 << RCC_PLLCFGR_PLLM_Pos);
	RCC->PLLCFGR |= (120 << RCC_PLLCFGR_PLLN_Pos);
	RCC->PLLCFGR |= (0 << RCC_PLLCFGR_PLLP_Pos);
	RCC->PLLCFGR |= (6 << RCC_PLLCFGR_PLLQ_Pos);
#endif

	MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC, RCC_PLLCFGR_PLLSRC_HSE);
	SET_BIT(RCC->CR, RCC_CR_PLLON);
	while(READ_BIT(RCC->CR, RCC_CR_PLLRDY)!= (RCC_CR_PLLRDY)) {}

	MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
	while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}
	//SystemCoreClockUpdate();
	//system_clk = SystemCoreClock;
	system_clk = 168000000;
	//system_clk = 120000000;
}

#if 0
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

	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {}

	SystemCoreClockUpdate();
	//system_clk = SystemCoreClock;
	system_clk = 168000000;
}
#endif

void rcc_enable(const rcc_periph_id_t periph_id)
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

	case tim6_id:	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;	
					break;

	case tim7_id:	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;	
					break;

	case spi1_id:	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;	
					break;

	case spi2_id:	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;	
					break;

	default:		return;
	}
}

rcc_clk_t rcc_get_system_clk()
{
	return system_clk;
}

rcc_clk_t rcc_get_ahb1_clk()
{
	return system_clk / rcc_ahb1_div;
}

rcc_clk_t rcc_get_apb1_clk()
{
	return system_clk / rcc_apb1_div;
}

rcc_clk_t rcc_get_apb2_clk()
{
	return system_clk / rcc_apb2_div;
}
