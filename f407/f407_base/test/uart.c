#include "uart.h"

void uart_init(USART_TypeDef *uart, uint32_t baud)
{
	GPIOA->MODER |= GPIO_MODER_MODE9_1 | GPIO_MODER_MODE10_1;
	//GPIOA->PUPDR |= GPIO_PUPDR_PUPD10_0;
	GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9_0 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_2;
	GPIOA->AFR[1] |= GPIO_AFRH_AFSEL10_0 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_2;
	uart->BRR = (SystemCoreClock / 2) / (baud);
	uart->CR1 = USART_CR1_UE;
}

void uart_enable(USART_TypeDef *uart)
{
	uart->CR1 |= USART_CR1_TE | USART_CR1_RE;
}

void uart_int_rx_enable(USART_TypeDef *uart)
{
	uart->CR1 |= USART_CR1_RXNEIE;
}

void uart_put_char(USART_TypeDef *uart, char c)
{
	while(!(uart->SR & USART_SR_TXE)) {}
	uart->DR = c;
}

void uart_put_str(USART_TypeDef *uart, char *str)
{
	while(*str) {
		while(!(uart->SR & USART_SR_TXE)) {}
		uart->DR = *str;
		str++;
	}
}
