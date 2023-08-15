#ifndef SPI_H_SENTRY
#define SPI_H_SENTRY

#include <stm32f4xx.h>

#include "rcc.h"
#include "nvic.h"
#include "general.h"
#include "gpio.h"

typedef uint32_t spi_speed_t;
typedef uint32_t spi_bus_div_t;

enum {
	spi1_gpio_port = gpio_port_a,
	spi1_nss_pin 	= 4,
	spi1_sck_pin 	= 5,
	spi1_miso_pin 	= 6,
	spi1_mosi_pin 	= 7,
	spi1_bus_div 	= rcc_apb2_div,
	spi1_irq_prior 	= 0
};

enum {
	spi2_gpio_port = gpio_port_b,
	spi2_nss_pin 	= 12,
	spi2_sck_pin 	= 13,
	spi2_miso_pin 	= 14,
	spi2_mosi_pin 	= 15,
	spi2_bus_div 	= rcc_apb1_div,
	spi2_irq_prior 	= 0
};

typedef enum {
	spi_nss_off = 1,
	spi_nss_on 	= 0
} spi_nss_state_t;

typedef enum {
	spi_cpol_rise = 0,
	spi_cpol_fall = 1
} spi_cpol_t;

typedef enum {
	spi_cpha_second_front 	= 0,
	spi_cpha_first_front 	= 1
} spi_cpha_t;

typedef enum {
	spi_mode_slave 	= 0,
	spi_mode_master = 1
} spi_mode_t;

typedef enum {
	spi_pwr_off = 0,
	spi_pwr_on 	= 1
} spi_pwr_state_t;

typedef enum {
	spi_spd_div_2 	= 0,
	spi_spd_div_4	= 1,
	spi_spd_div_8	= 2,
	spi_spd_div_16	= 3,
	spi_spd_div_32	= 4,
	spi_spd_div_64	= 5,
	spi_spd_div_128	= 6,
	spi_spd_div_256	= 7
} spi_spd_div_t;

typedef enum {
	spi_frame_len_8,
	spi_frame_len_16
} spi_frame_len_t;

typedef enum {
	spi_lsbfirst,
	spi_msbfirst
} spi_bit_order_t;

typedef enum {
	spi_1,
	spi_2,
	spi_3
} spi_num_t;

typedef struct {
	spi_num_t 		num;
	spi_mode_t 		mode;
	spi_cpol_t 		cpol;
	spi_cpha_t 		cpha;
	spi_spd_div_t	spd_div;
	spi_nss_state_t nss;
	spi_frame_len_t frame_len;
	spi_bit_order_t bit_order;
	spi_pwr_state_t	pwr;
} spi_t;

void spi_init(spi_t *spi_setup);
void spi_send_char(spi_t *spi_setup, const char c);
void spi_swap_char(spi_t *spi_setup, const char send_c, char *recv_c);
void spi_recv_char(spi_t *spi_setup, char *c);
#if 1
void spi_send_buff(spi_t *spi_setup, const void *buff, const uint32_t byte_num);
void spi_send_str(spi_t *spi_setup, const char *str);
#endif

#endif
