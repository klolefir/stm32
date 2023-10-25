#include "usart.h"
#include "nvic.h"
#include "gpio.h"
#include "rcc.h"
#include "kestring.h"
#include "stdlib.h"

static void usart_gpio_init(const usart_t *usart_set);
static void usart_rcc_init(const usart_t *usart_set);
static void irq_init(const usart_t *usart_set);
static void tx_init(const usart_t *usart_set);
static void rx_init(const usart_t *usart_set);
static void baud_init(const usart_t *usart_set);
static rcc_periph_id_t switch_rcc_id(const usart_t *usart_set);
static USART_TypeDef *switch_usart(const usart_t *usart_set);
static nvic_irqn_t switch_irqn(const usart_t *usart_set);
static nvic_irq_prior_t switch_irq_prior(const usart_t *usart_set);
static rcc_bus_div_t switch_bus_div(const usart_t *usart_set);

void usart_init(const usart_t *usart_set)
{
	usart_gpio_init(usart_set);
	usart_rcc_init(usart_set);
	irq_init(usart_set);
	tx_init(usart_set);
	rx_init(usart_set);
	baud_init(usart_set);
	usart_enable(usart_set);
}

void usart_enable(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart->CR1 |= USART_CR1_UE;
}

void usart_disable(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart->CR1 &= ~USART_CR1_UE;
}

void usart_tx_enable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->tx_state = usart_tx_on;
	usart->CR1 |= USART_CR1_TE;
}

void usart_rx_enable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->rx_state = usart_rx_on;
	usart->CR1 |= USART_CR1_RE;
}

void usart_tx_disable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->tx_state = usart_tx_off;
	usart->CR1 &= ~USART_CR1_TE;
}

void usart_rx_disable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->rx_state = usart_rx_off;
	usart->CR1 &= ~USART_CR1_RE;
}

void usart_tx_irq_enable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->tx_irq_state = usart_tx_irq_on;
	usart->CR1 |= USART_CR1_TXEIE;
}

void usart_rx_irq_enable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->rx_irq_state = usart_rx_irq_on;
	usart->CR1 |= USART_CR1_RXNEIE;
}

void usart_tx_irq_disable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->tx_irq_state = usart_tx_irq_off;
	usart->CR1 &= ~USART_CR1_TXEIE;
}

void usart_rx_irq_disable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->rx_irq_state = usart_rx_irq_off;
	usart->CR1 &= ~USART_CR1_RXNEIE;
}

void usart_put_char(const usart_t *usart_set, const char c)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	while(!(usart->SR & USART_SR_TXE)) {}
	usart->DR = c;
}

void usart_put_uint32(const usart_t *usart_set, const uint32_t data)
{
	char data_str[5];
	kememcpy(data_str, &data, sizeof(uint32_t));
	data_str[4] = '\0';
	usart_put_str(usart_set, data_str);
}


void usart_put_str(const usart_t *usart_set, const char *str)
{
	while(*str) {
		usart_put_char(usart_set, *str);
		str++;
	}
}

void usart_put_buff(const usart_t *usart_set, const void *buff, const uint32_t byte_num)
{
	uint8_t *buff_ptr = (uint8_t *)buff;
	int i;
	for(i = 0; i < byte_num; i++)
		usart_put_char(usart_set, buff_ptr[i]);
}

usart_rx_status_t usart_get_rx_status(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	if((usart->SR & USART_SR_RXNE))
		return usart_rx_rdy;
	else
		return usart_rx_nrdy;
}

usart_tx_status_t usart_get_tx_status(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	if((usart->SR & USART_SR_TXE))
		return usart_tx_rdy;
	else
		return usart_tx_nrdy;
}

void usart_get_char(const usart_t *usart_set, char *c)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	/*while(!(usart->SR & USART_SR_RXNE)) {}*/
	*c = usart->DR;
}

void usart_gpio_init(const usart_t *usart_set)
{
	gpio_t tx_pin, rx_pin;

	tx_pin.mode 	= gpio_mode_alt;
	tx_pin.pupd 	= gpio_pupd_no;

	rx_pin.mode 	= gpio_mode_alt;
	rx_pin.pupd 	= gpio_pupd_pu;

	switch(usart_set->usart) {
	case usart_num_1: 	tx_pin.port = usart1_gpio_port;
						tx_pin.mask = usart1_tx_pin; 
						tx_pin.alt 	= gpio_alt_usart1;
						rx_pin.port = usart1_gpio_port;
						rx_pin.mask = usart1_rx_pin; 
						rx_pin.alt 	= gpio_alt_usart1;
						break;
	case usart_num_2: 	tx_pin.port = usart2_gpio_port;
						tx_pin.mask = usart2_tx_pin;
						tx_pin.alt 	= gpio_alt_usart2;
						rx_pin.port = usart2_gpio_port;
						rx_pin.mask = usart2_rx_pin;
						rx_pin.alt 	= gpio_alt_usart2;
						break;
	default:			return;
	}

	gpio_init(&rx_pin);
	gpio_init(&tx_pin);
}

void usart_rcc_init(const usart_t *usart_set)
{
	rcc_periph_id_t usart_id = switch_rcc_id(usart_set);
	rcc_enable(usart_id);
}

void irq_init(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	nvic_irqn_t usart_irqn = switch_irqn(usart_set);
	nvic_irq_prior_t usart_irq_prior = switch_irq_prior(usart_set);

	nvic_enable_irq(usart_irqn, usart_irq_prior);

	switch(usart_set->tx_irq_state) {
	case usart_tx_irq_off:	usart->CR1 &= ~USART_CR1_TXEIE;
							break;
	case usart_tx_irq_on:	usart->CR1 |= USART_CR1_TXEIE;
							break;
	}

	switch(usart_set->rx_irq_state) {
	case usart_rx_irq_off:	usart->CR1 &= ~USART_CR1_RXNEIE;
							break;
	case usart_rx_irq_on:	usart->CR1 |= USART_CR1_RXNEIE;
							break;
	}
}

rcc_periph_id_t switch_rcc_id(const usart_t *usart_set)
{
	switch(usart_set->usart) {
	case usart_num_1: 	return usart1_id;
	case usart_num_2: 	return usart2_id;
	default:			return none_id;	
	}
}

nvic_irqn_t switch_irqn(const usart_t *usart_set)
{
	switch(usart_set->usart) {
	case usart_num_1: 	return usart1_irqn;
	case usart_num_2: 	return usart2_irqn;
	default:			return none_irqn;	
	}
}

nvic_irq_prior_t switch_irq_prior(const usart_t *usart_set)
{
	switch(usart_set->usart) {
	case usart_num_1: 	return usart1_irq_prior;
	case usart_num_2: 	return usart2_irq_prior;
	default:			return none_irq_prior;
	}
}

rcc_bus_div_t switch_bus_div(const usart_t *usart_set)
{
	switch(usart_set->usart) {
	case usart_num_1: 	return usart1_bus_div;
	case usart_num_2: 	return usart2_bus_div;
	default:			return 1;
	}
}

void tx_init(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	switch(usart_set->tx_state) {
	case usart_tx_off:	usart->CR1 &= ~USART_CR1_TE;
						break;
	case usart_tx_on:	usart->CR1 |= USART_CR1_TE;
						break;
	}
}

void rx_init(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	switch(usart_set->rx_state) {
	case usart_rx_off:	usart->CR1 &= ~USART_CR1_RE;
						break;
	case usart_rx_on:	usart->CR1 |= USART_CR1_RE;
						break;
	}
}

void baud_init(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	rcc_bus_div_t usart_bus_div = switch_bus_div(usart_set);
	uint32_t clk = rcc_get_system_clk();
	usart->BRR = (clk / usart_bus_div) / (usart_set->baud);
}

USART_TypeDef *switch_usart(const usart_t *usart_set)
{
	switch(usart_set->usart) {
	case usart_num_1: 	return USART1;
	case usart_num_2: 	return USART2;
	default:			return NULL;
	}
}
