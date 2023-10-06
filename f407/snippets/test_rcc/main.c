#include "main.h"

#if 0
void usart_put_uint32(uint32_t data);
#endif
void flash_put_status(flash_status_t status);
void flash_write_test();
void flash_read_test();

int main(void)
{
	rcc_init();
	enable_irq();
	gpio_init(&led1_pin);
	systick_init(1000);
	
	while(1) {
		gpio_tgl(&led1_pin);
		systick_delay_ms(1000);
	}
}

void flash_put_status(flash_status_t status)
{
	switch(status) {
	case flash_lock_err: 		usart_put_str(&usart1, "Flash lock!\r");
								break;
	case flash_bad_addr_err: 	usart_put_str(&usart1, "Flash bad addr!\r");
								break;
	case flash_ok:				usart_put_str(&usart1, "Flash ok!\r");
								break;
	default:					usart_put_str(&usart1, "Flash unknown status!\r");
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
