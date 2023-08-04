#ifndef INIT_H_SENTRY
#define INIT_H_SENTRY

#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "flash.h"

extern usart_t usart1;
extern tim_t usart_tim;
extern gpio_t led1_pin;

typedef uint32_t buff_size_t;

enum {
	//main_addr 	= 0x08008000,
	//main_sector = main_addr
	main_addr = flash_sector9_addr,
	main_sector = main_addr
};

enum {
	req_buff_len = 128,
	dec_buff_len = 128,
	ans_buff_len = 128
};

typedef struct {
	buff_size_t buff[req_buff_len];
	uint32_t cnt;
} req_buff_t;

typedef struct {
	buff_size_t buff[dec_buff_len];
	uint32_t cnt;
} dec_buff_t;

typedef struct {
	buff_size_t buff[ans_buff_len];
	uint32_t cnt;
} ans_buff_t;

#endif
