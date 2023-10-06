#include "stm32f1xx.h"

int main()
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH = GPIO_CRH_MODE13_1;
	GPIOC->BSRR = GPIO_BSRR_BR13;
	while(1)
	{}
}
