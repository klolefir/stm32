#ifndef SPI_H_SENTRY
#define SPI_H_SENTRY

#include <stm32f4xx.h>

typedef uint32_t spi_speed_t;

enum {
	spi1_gpio_port = gpio_port_a,
	spi1_nss_pin 	= 4,
	spi1_sck_pin 	= 5,
	spi1_miso_pin 	= 6,
	spi1_mosi_pin 	= 7,
	spi1_bus_div 	= rcc_apb2_dib,
	spi1_irq_prior 	= 0
};

enum {
	spi2_gpio_port = gpio_port_b,
	spi2_nss_pin 	= 12,
	spi2_sck_pin 	= 13,
	spi2_miso_pin 	= 14,
	spi2_mosi_pin 	= 15,
	spi2_bus_div 	= rcc_apb2_dib,
	spi2_irq_prior 	= 0
};

typedef enum {
	spi_nss_on,
	spi_nss_off
} spi_nss_state_t;

typedef enum {
	spi_cpol_rise = 0,
	spi_cpol_fall = 1
} spi_cpol_t;

typedef enum {
	spi_cpha_scd_front 	= 0
	spi_cpha_fst_front 	= 1
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
	spi_1,
	spi_2,
	spi_3
} spi_num_t;

typedef struct {
	spi_num_t 		num;
	spi_mode_t 		mode;
	spi_cpol_t 		cpol;
	spi_cpha_t 		cpha;
	spi_nss_state_t nss;
	spi_pwr_state_t	pwr;
} spi_t;

void spi_init(spi_t spi_st);

#endif
