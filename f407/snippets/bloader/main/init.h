#ifndef INIT_H_SENTRY
#define INIT_H_SENTRY

#include "tim.h"
#include "usart.h"
#include "gpio.h"

usart_t usart1 = { 
	.usart = USART1, 
	.baud = usart_baud_115200, 
	.rx_st = usart_rx_on,
	.tx_st = usart_tx_on,
	.tx_int_st = usart_tx_int_off,
	.rx_int_st = usart_rx_int_off
};

tim_t usart_tim = {
	.tim_num 	= tim_6,
	.psc 		= 21000,
	.arr 		= 100,
	.irq_st 	= tim_irq_on,
	.pwr_st 	= tim_pwr_on
};

gpio_t led1_pin = {
	.port = gpio_port_a,
	.mask = 6,
	.mode = gpio_mode_output,
	.otype = gpio_otype_pp,
	.ospeed = gpio_ospeed_low,
	.pupd = gpio_pupd_pu,
	.alt = gpio_alt_system
};

enum {
	req_buff_len = 1024,
	dec_buff_len = 1024,
	ans_buff_len = 1024
};

typedef struct {
	char buff[req_buff_len];
	uint32_t cnt;
} req_buff_t;

typedef struct {
	char buff[dec_buff_len];
	uint32_t cnt;
} dec_buff_t;

typedef struct {
	char buff[ans_buff_len];
	uint32_t cnt;
} ans_buff_t;


#endif
