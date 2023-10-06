#include <stdio.h>
#include "main.h"

int main(void)
{
	rcc_init();
	enable_irq();

	gpio_init(&led1_pin);
	usart_init(&usart1);

	char data[10];
	//sprintf(data, "%d", 10);
	printf("abc");
	usart_put_str(&usart1, data);
	
	while(1) {
	}
}

#if 0
void usart_put_uint32(uint32_t data)
{
	char data_str[6];
	memcpy(data_str, &data, sizeof(uint32_t));
	data_str[4] = '\r';
	data_str[5] = '\0';
	usart_put_str(&usart1, data_str);
}
#endif
