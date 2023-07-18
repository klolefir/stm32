#ifndef USART_H_SENTRY
#define USART_H_SENTRY

#include "stm32f4xx.h"

#define USART1_GPIO_PORT GPIOA
#define USART2_GPIO_PORT GPIOB

enum {
	usart1_gpio_port = gpio_port_a,
	usart2_gpio_port = gpio_port_d,
	usart1_tx_pin = 9,
	usart1_rx_pin = 10,
	usart2_tx_pin = 5,
	usart2_rx_pin = 6,
	usart1_bus_div = rcc_apb2_div,
	usart2_bus_div = rcc_apb1_div
};

typedef enum {
	usart_baud_9600 	= 9600,
	usart_baud_115200 	= 115200
} usart_baud_t;

typedef enum {
	usart_rx_off 	= 0,
	usart_rx_en		= 1
} usart_rx_state_t;

typedef enum {
	usart_tx_off 	= 0,
	usart_tx_on 	= 1
} usart_tx_state_t;

typedef enum {
	usart_rx_int_off 	= 0,
	usart_rx_int_on		= 1
} usart_rx_int_state_t;

typedef enum {
	usart_tx_int_off	= 0,
	usart_tx_int_on		= 1
} usart_tx_int_state_t;

typedef struct {
	USART_TypeDef 			*usart;
	usart_baud_t			baud;
	usart_rx_state_t		rx_st 		: 1;
	usart_rx_state_t		tx_st 		: 1;
	usart_rx_int_state_t	rx_int_st 	: 1;
	usart_tx_int_state_t	tx_int_st	: 1;
} usart_t;

void usart_init(usart_t *usart);

void usart_put_char(usart_t *usart_st, const char c);
void usart_put_str(usart_t *usart_st, const char *str);
void usart_put_buff(usart_t *usart_st, const char *buff, const uint32_t len);
void usart_get_char(usart_t *usart_st, char *c);
//void usart_get_str(usart_t *usart_st, char *str, char term = '\r');

void usart_enable(usart_t *usart_st);
void usart_disable(usart_t *usart_st);

void usart_rx_enable(usart_t *usart_st);
void usart_rx_disable(usart_t *usart_st);

void usart_tx_enable(usart_t *usart_st);
void usart_tx_disable(usart_t *usart_st);

void usart_rx_int_enalbe(usart_t *usart_st);
void usart_rx_int_disable(usart_t *usart_st);

void usart_tx_int_enalbe(usart_t *usart_st);
void usart_tx_int_disable(usart_t *usart_st);

#endif