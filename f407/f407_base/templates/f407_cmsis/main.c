#include <stdint.h>
#include "stm32f4xx.h"

#define LED1_PIN 6
#define LED2_PIN 7

int main(void)
{
	RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOAEN_Pos);
	
	GPIOA->MODER |= (1 << GPIO_MODER_MODER6_Pos);
	GPIOA->MODER |= (1 << GPIO_MODER_MODER7_Pos);
	

	while(1) {
		GPIOA->ODR ^= (1 << LED1_PIN);
		GPIOA->ODR ^= (1 << LED2_PIN);
		for(uint32_t i = 0; i < 1000000; i++) {}
	}
}
