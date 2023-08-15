#include "main.h"

int main(void)
{
	rcc_init();
	enable_irq();

	usart_init(&usart1);

	spi_init(&spi1);
	spi_init(&spi2);
	
	char spi1_send_c = '1', spi2_send_c = '2', spi1_recv_c, spi2_recv_c;
	spi_send_char(&spi2, spi2_send_c);
	spi_swap_char(&spi1, spi1_send_c, &spi1_recv_c);
	spi_recv_char(&spi2, &spi2_recv_c);

	usart_put_char(&usart1, spi1_recv_c);
	usart_put_char(&usart1, spi2_recv_c);

	while(1) {
	}
}
