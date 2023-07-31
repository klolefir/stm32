#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stm32f4xx.h>

#include "gpio.h"
#include "rcc.h"
#include "general.h"
#include "systick.h"
#include "usart.h"
#include "tim.h"

void enable_irq();

volatile uint32_t usart_ticks = 0;

typedef enum {
	idle_state,
	receive_state,
	decode_state,
	handle_state,
	respond_state,
	purge_state,
	reset_state
#if 0
	init,
	deinit
#endif
} fsm_state_t;

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

typedef enum {
	recv_st_ok,
	recv_st_bad
} recv_st_t;

typedef enum {
	handle_st_ok,
	handle_st_rst,
	handle_st_bad
} handle_st_t;

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

static gpio_t led1_pin = { 
	.port = gpio_port_a, 
	.mask = 6, 
	.mode = gpio_mode_output, 
	.otype = gpio_otype_pp,
	.ospeed = gpio_ospeed_low,
	.pupd = gpio_pupd_pu,
	.alt = gpio_alt_system
};

void enable_irq()
{
	__enable_irq();
}
