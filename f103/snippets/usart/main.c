#include "stm32f1xx.h"

int main()
{
	SystemInit();
	SystemCoreClockUpdate();
	char c = 'a';
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	USART2->CR1 = USART_CR1_UE;
	USART2->BRR = SystemCoreClock / 115200;
	USART2->CR1 |= USART_CR1_TE;

	GPIOA->CRL &= ~GPIO_CRL_MODE2;
	GPIOA->CRL &= ~GPIO_CRL_CNF2;
	GPIOA->CRL |= GPIO_CRL_MODE2_1;
	GPIOA->CRL |= GPIO_CRL_CNF2_1;

	GPIOA->CRL &= ~GPIO_CRL_MODE3;
	GPIOA->CRL &= ~GPIO_CRL_CNF3;
	GPIOA->CRL |= GPIO_CRL_CNF3_1;
	/*GPIOA->BSRR = GPIO_BSRR_BS3;*/

	USART2->CR1 |= USART_CR1_RE;

	while(1) {
#if 1
		while(!(USART2->SR & USART_SR_RXNE))
		{}
		c = USART2->DR;

#endif
		while(!(USART2->SR & USART_SR_TXE))
		{}

		USART2->DR = c;
	}
}
