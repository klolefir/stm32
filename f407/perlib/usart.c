#include "usart.h"
#include "gpio.h"
#include "rcc.h"

static void usart_init_gpio(usart_t *usart_st)
{
	USART_TypeDef *usart = usart_st->usart;
	gpio_t tx_pin, rx_pin;

	tx_pin.mode 	= gpio_mode_alt;
	tx_pin.pupd 	= gpio_pupd_no;

	rx_pin.mode 	= gpio_mode_alt;
	rx_pin.pupd 	= gpio_pupd_no;

	if(usart == USART1) {
		rcc_enable(usart1_gpio_port);

		tx_pin.port = usart1_gpio_port;
		tx_pin.mask = usart1_tx_pin; 
		tx_pin.alt 	= gpio_alt_usart1;

		rx_pin.port = usart1_gpio_port;
		rx_pin.mask = usart1_rx_pin; 
		rx_pin.alt 	= gpio_alt_usart1;
	} else if(usart == USART2) {
		rcc_enable(usart2_gpio_port);

		tx_pin.port = usart2_gpio_port;
		tx_pin.mask = usart2_tx_pin;
		tx_pin.alt 	= gpio_alt_usart2;

		rx_pin.port = usart2_gpio_port;
		rx_pin.mask = usart2_rx_pin;
		rx_pin.alt 	= gpio_alt_usart2;
	} else {
		return;
	}

	gpio_init(&rx_pin);
	gpio_init(&tx_pin);
}

void usart_init(usart_t *usart_st)
{
	usart_init_gpio(usart_st);

	uint8_t rcc_bus_div = usart1_bus_div;
	USART_TypeDef *usart = usart_st->usart;
	if(usart == USART1) {
		rcc_enable(usart1_id);
		rcc_bus_div = usart1_bus_div;
	}
	else if(usart == USART2) {
		rcc_enable(usart2_id);
		rcc_bus_div = usart2_bus_div;
	}

	usart->CR1 |= (usart_st->tx_st << USART_CR1_TE_Pos);
	usart->CR1 |= (usart_st->rx_st << USART_CR1_RE_Pos);
	usart->CR1 |= (usart_st->tx_int_st << USART_CR1_TXEIE_Pos);
	usart->CR1 |= (usart_st->rx_int_st << USART_CR1_RXNEIE_Pos);

	usart->BRR = (SystemCoreClock / rcc_bus_div) / (usart_st->baud);
	usart->CR1 |= USART_CR1_UE;
}

void usart_enable(usart_t *usart_st)
{
	usart_st->tx_st = usart_tx_on;
	usart_st->rx_st = usart_rx_on;
	USART_TypeDef *usart = usart_st->usart;
	usart->CR1 |= USART_CR1_TE | USART_CR1_RE;
}

void usart_tx_enable(usart_t *usart_st)
{
	usart_st->tx_st = usart_tx_on;
	USART_TypeDef *usart = usart_st->usart;
	usart->CR1 |= USART_CR1_TE;
}

void usart_rx_enable(usart_t *usart_st)
{
	usart_st->rx_st = usart_rx_on;
	USART_TypeDef *usart = usart_st->usart;
	usart->CR1 |= USART_CR1_RE;
}

void usart_tx_disable(usart_t *usart_st)
{
	usart_st->tx_st = usart_tx_off;
	USART_TypeDef *usart = usart_st->usart;
	usart->CR1 &= ~USART_CR1_TE;
}

void usart_rx_disable(usart_t *usart_st)
{
	usart_st->rx_st = usart_rx_off;
	USART_TypeDef *usart = usart_st->usart;
	usart->CR1 &= ~USART_CR1_RE;
}

void usart_tx_int_enable(usart_t *usart_st)
{
	usart_st->tx_int_st = usart_tx_int_on;
	USART_TypeDef *usart = usart_st->usart;
	usart->CR1 |= USART_CR1_TXEIE;
}

void usart_rx_int_enable(usart_t *usart_st)
{
	usart_st->rx_int_st = usart_rx_int_on;
	USART_TypeDef *usart = usart_st->usart;
	usart->CR1 |= USART_CR1_RXNEIE;
}

void usart_tx_int_disable(usart_t *usart_st)
{
	usart_st->tx_int_st = usart_tx_int_off;
	USART_TypeDef *usart = usart_st->usart;
	usart->CR1 &= ~USART_CR1_TXEIE;
}

void usart_rx_int_disable(usart_t *usart_st)
{
	usart_st->rx_int_st = usart_rx_int_off;
	USART_TypeDef *usart = usart_st->usart;
	usart->CR1 &= ~USART_CR1_RXNEIE;
}

void usart_put_char(usart_t *usart_st, const char c)
{
	USART_TypeDef *usart = usart_st->usart;
	while(!(usart->SR & USART_SR_TXE)) {}
	usart->DR = c;
}

void usart_put_str(usart_t *usart_st, const char *str)
{
	USART_TypeDef *usart = usart_st->usart;
	while(*str) {
		while(!(usart->SR & USART_SR_TXE)) {}
		usart->DR = *str;
		str++;
	}
}

void usart_put_buff(usart_t *usart_st, const char *buff, const uint32_t len)
{
	USART_TypeDef *usart = usart_st->usart;
	for(uint32_t i = 0; i < len; i++) {
		while(!(usart->SR & USART_SR_TXE)) {}
		usart->DR = buff[i];
	}
}

void usart_get_char(usart_t *usart_st, char *c)
{
	USART_TypeDef *usart = usart_st->usart;
	while(!(usart->SR & USART_SR_RXNE)) {}
	*c = usart->DR;
}
