#ifndef OPERATOR_H_SENTRY
#define OPERATOR_H_SENTRY

#include "init.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

extern gpio_t debugpin;

typedef enum {
	recv_state_ok,
	recv_state_bad
} recv_state_t;

enum {
	operator_usart_num 		= usart_num_1,
	operator_usart_baud 	= usart_baud_115200
};

enum {
	operator_delay = 2 /* us */
};

enum {
	operator_timer_num 	= tim_num_7,
	operator_timer_freq	= 1000
};

void operator_init();
recv_state_t operator_receive(buff_t *buff_set);
void operator_transmit(const buff_t *buff_set);

#endif
