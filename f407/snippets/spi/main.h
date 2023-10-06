#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stm32f4xx.h>

#include "gpio.h"
#include "rcc.h"
#include "usart.h"
#include "general.h"
#include "flash.h"
#include "spi.h"

#define VECTOR_TAB_SRAM
#define LED1_PIN 6
#define LED2_PIN 7

void enable_irq();

static usart_t usart1 = { 
	.usart = USART1, 
	.baud = usart_baud_115200, 
	.rx_st = usart_rx_on,
	.tx_st = usart_tx_on,
	.tx_int_st = usart_tx_int_off,
	.rx_int_st = usart_rx_int_off
};

#if 0
static gpio_t led1_pin = { 
	.port = gpio_port_a, 
	.mask = 6, 
	.mode = gpio_mode_output, 
	.otype = gpio_otype_pp,
	.ospeed = gpio_ospeed_low,
	.pupd = gpio_pupd_pu,
	.alt = gpio_alt_system
};
#endif

static gpio_t cs_pin = { 
	.port = gpio_port_a, 
	.mask = 9,
	.mode = gpio_mode_output, 
	.otype = gpio_otype_pp,
	.ospeed = gpio_ospeed_vhigh,
	.pupd = gpio_pupd_pu,
	.alt = gpio_alt_system
};

static spi_t spi1 = {
	.num = spi_1,
	.mode = spi_mode_master,
	.cpol = spi_cpol_rise,
	.cpha = spi_cpha_first_front,
	.spd_div = spi_spd_div_256,
	.nss = spi_nss_off,
	.frame_len = spi_frame_len_8,
	.bit_order = spi_lsbfirst,
	.pwr = spi_pwr_on
};

static spi_t spi2 = {
	.num = spi_2,
	.mode = spi_mode_slave,
	.cpol = spi_cpol_rise,
	.cpha = spi_cpha_first_front,
	.spd_div = spi_spd_div_256,
	.nss = spi_nss_off,
	.frame_len = spi_frame_len_8,
	.bit_order = spi_lsbfirst,
	.pwr = spi_pwr_on
};

void enable_irq()
{
	__enable_irq();
}
