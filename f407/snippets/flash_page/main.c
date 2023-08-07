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
	usart_init(&usart1);

	const uint32_t page_addr = flash_sector9_addr;
	const uint32_t page_size = 256; /*bytes*/
	const uint32_t data_len = 64;
	uint32_t write_data[data_len];
	uint32_t read_data[data_len];
	flash_status_t status;
	uint32_t i;

	for(i = 0; i < data_len; i++)
		write_data[i] = 0xEFBEADDE;

	/********************write************************/
#if 1
	flash_unlock();
	status = flash_erase(page_addr);
	if(status == flash_ok)
		status = flash_write_page(page_addr, write_data, page_size);
	flash_lock();
	/********************write************************/

	/********************read************************/
	if(status == flash_ok)
#endif 
		status = flash_read_page(page_addr, read_data, page_size);
	if(status == flash_ok) {
		for(i = 0; i < data_len; i++) {
			usart_put_uint32(&usart1, read_data[i]);
		}
	}
	/********************read************************/

	flash_put_status(status);

	usart_put_uint32(&usart1, rcc_get_system_clk() / 1000000);
	
	while(1) {
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
