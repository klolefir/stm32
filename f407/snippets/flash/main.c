#include "main.h"

void usart_put_uint32(uint32_t data);
void flash_put_status(flash_status_t status);
void flash_write_test();
void flash_read_test();

int main(void)
{
	rcc_init();
	enable_irq();

	gpio_init(&led1_pin);
	usart_init(&usart1);

	const uint32_t page_addr = flash_sector9_addr + 0x100;
	const uint32_t write_data = 0xDEADBEEF;
	uint32_t read_data;
	flash_status_t status;

	/********************write***********************/
	status = flash_write(page_addr, write_data);

	usart_put_uint32(write_data);
	flash_put_status(status);
	/********************write***********************/

	/********************read***********************/
	status = flash_read(page_addr, &read_data);

	usart_put_uint32(read_data);
	flash_put_status(status);
	/********************read***********************/
	
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

void usart_put_uint32(uint32_t data)
{
	char data_str[6];
	memcpy(data_str, &data, sizeof(uint32_t));
	data_str[4] = '\r';
	data_str[5] = '\0';
	usart_put_str(&usart1, data_str);
}
