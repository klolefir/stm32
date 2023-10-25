#include "operator.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

static usart_t usart;
static tim_t tim;
gpio_t debugpin;

void init_interface();
void init_timer();
void init_debugpin();

recv_state_t operator_receive(buff_t *buff_set)
{
	recv_state_t recv_state = recv_state_bad;

	uint32_t *req_cnt = &(buff_set->cnt);
	buff_size_t *req_buff = (buff_set->buff);

	static uint32_t timer = 0;
	uint32_t usart_ticks = tim_get_ticks(&tim);
	char c;

	usart_rx_status_t rx_status = usart_get_rx_status(&usart);
	if(rx_status == usart_rx_rdy) {
		usart_get_char(&usart, &c);
		req_buff[*req_cnt] = c;
		(*req_cnt)++;
		timer = usart_ticks + operator_delay; /* delay ms */
	}

	if(((timer) && (usart_ticks >= timer) && (*req_cnt)) ||
		(*req_cnt >= max_buff_len))
	{
		recv_state = recv_state_ok;
		timer = 0;
	}

	return recv_state;
}

void operator_transmit(const buff_t *buff_set)
{
	const uint32_t ans_cnt = buff_set->cnt;
	const buff_size_t *ans_buff = (buff_set->buff);

	if(ans_cnt)
		usart_put_buff(&usart, ans_buff, ans_cnt);
}

void operator_init()
{
	init_interface();
	init_timer();
}

void init_interface()
{
	usart.usart 		= operator_usart_num;
	usart.baud 			= operator_usart_baud;
	usart.tx_state 		= usart_tx_on;
	usart.rx_state 		= usart_rx_on;
	usart.tx_irq_state = usart_tx_irq_off;
	usart.rx_irq_state = usart_rx_irq_off;
	usart_init(&usart);
}

void init_timer()
{
	uint32_t psc = rcc_get_apb1_clk() / operator_timer_freq;
	tim.tim 			= operator_timer_num;
	tim.psc 			= psc;
	tim.arr 			= 10;
	tim.irq_state 		= tim_irq_on;
	tim_init(&tim);
}
