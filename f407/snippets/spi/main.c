#include "main.h"
#include "systick.h"

int main(void)
{
	char spi1_send_c = 'a', spi2_send_c = 'a', spi1_recv_c, spi2_recv_c;
	rcc_init();
	enable_irq();

	usart_init(&usart1);
	systick_init(1000);
	systick_enable();

	gpio_init(&cs_pin);
	spi_init(&spi1);
	spi_init(&spi2);
	systick_delay_ms(1);
	
	gpio_clr(&cs_pin);
	spi_send_char(&spi2, spi2_send_c);
	spi_swap_char(&spi1, spi1_send_c, &spi1_recv_c);
	spi_recv_char(&spi2, &spi2_recv_c);
	systick_delay_ms(1);
	gpio_set(&cs_pin);

	usart_put_char(&usart1, spi1_recv_c);
	usart_put_char(&usart1, spi2_recv_c);

	while(1) {
	}
}
