#include <stdint.h>
#include "stm32f4xx.h" 

#define VECTOR_TAB_SRAM

#define LED1_PIN 6
#define LED2_PIN 7
void rcc_init();
void rcc_deinit();

volatile uint32_t ticks = 0;

void systick_handler()
{
	ticks++;
}

void delay_ms(uint32_t ms)
{
	uint32_t start = ticks;
	uint32_t end = start + ms;

	if(end < start)
		while(ticks > start) {}

	while(ticks < end) {}
}

int main(void)
{
	rcc_deinit();
	rcc_init();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
	__enable_irq();

	RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOAEN_Pos);
	
	GPIOA->MODER |= (1 << GPIO_MODER_MODER6_Pos);
	GPIOA->MODER |= (1 << GPIO_MODER_MODER7_Pos);
	
	while(1) {
		GPIOA->ODR ^= (1 << LED1_PIN);
		GPIOA->ODR ^= (1 << LED2_PIN);
		delay_ms(1000);
	}
}

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

void rcc_init() {
	
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
}
