#include <stdio.h>
#include <stdint.h>
#include <stm32f4xx.h>

#include "rcc.h"
#include "usart.h"
#include "tim.h"

void enable_irq();

static volatile uint32_t usart_ticks = 0;

static usart_t usart1 = { 
	.usart = USART1, 
	.baud = usart_baud_115200, 
	.rx_st = usart_rx_on,
	.tx_st = usart_tx_on,
	.tx_int_st = usart_tx_int_off,
	.rx_int_st = usart_rx_int_off
};

static tim_t tim6 = {
	.tim_num 	= tim_6,
	.psc 		= 21000,
	.arr 		= 100,
	.irq_st 	= tim_irq_on,
	.pwr_st 	= tim_pwr_on
};

void enable_irq()
{
	__enable_irq();
}
