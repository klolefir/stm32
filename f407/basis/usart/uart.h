#ifndef UART_H_SENTRY
#define UART_H_SENTRY

#include "stm32f4xx.h"

void uart_init(USART_TypeDef *uart, uint32_t baud);
void uart_enable(USART_TypeDef *uart);
void uart_int_rx_enable(USART_TypeDef *uart);
void uart_put_char(USART_TypeDef *uart, char c);
void uart_put_str(USART_TypeDef *uart, char *str);


#endif
