#ifndef GPIO_H_SENTRY
#define GPIO_H_SENTRY

#include "stm32f4xx.h"
#include "rcc.h"

typedef enum {
	gpio_port_a = gpioa_id,
	gpio_port_b = gpiob_id,
	gpio_port_c = gpioc_id,
	gpio_port_d = gpiod_id
} gpio_port_t;

typedef enum {
	gpio_mode_input 	= 0b00,
	gpio_mode_output 	= 0b01,
	gpio_mode_alt 		= 0b10,
	gpio_mode_analog 	= 0b11
} gpio_mode_t;

typedef enum {
	gpio_otype_pp = 0,
	gpio_otype_od = 1
} gpio_otype_t;

typedef enum {
	gpio_ospeed_low = 0b00,
	gpio_ospeed_medium = 0b01,
	gpio_ospeed_high = 0b10,
	gpio_ospeed_vhigh = 0b11
} gpio_ospeed_t;

typedef enum {
	gpio_pupd_no = 0b00,
	gpio_pupd_pu = 0b01,
	gpio_pupd_pd = 0b10
} gpio_pupd_t;

typedef enum {
	gpio_lock_down,
	gpio_lock_up
} gpio_lock_t;

typedef enum {
	gpio_alt_system = 0b0000,
	gpio_alt_tim1	= 0b0001,
	gpio_alt_tim2	= gpio_alt_tim1,
	gpio_alt_tim3	= 0b0010,
	gpio_alt_tim4	= gpio_alt_tim3,
	gpio_alt_tim5 	= gpio_alt_tim3,
	gpio_alt_usart1	= 0b0111,
	gpio_alt_usart2	= gpio_alt_usart1,
	gpio_alt_usart3	= gpio_alt_usart1
} gpio_alt_t;

typedef struct {
	gpio_port_t 	port;
	uint32_t 		mask;
	gpio_mode_t 	mode 	: 2;
	gpio_otype_t 	otype 	: 1;
	gpio_ospeed_t 	ospeed 	: 2;
	gpio_pupd_t		pupd	: 2;
	gpio_alt_t		alt		: 4;
} gpio_t;

void gpio_init(gpio_t *pin);
void gpio_setout(gpio_t *pin);
void gpio_clrout(gpio_t *pin);
void gpio_tglout(gpio_t *pin);

#endif
