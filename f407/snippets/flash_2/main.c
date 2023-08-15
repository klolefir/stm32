#include "main.h"

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
	//const uint32_t page_addr = flash_sector2_addr;
	const uint32_t write_data = 0xABCDABCD;
	uint32_t read_data;
	flash_status_t status;

	/********************write***********************/
	flash_init();

	flash_unlock();
	status = flash_erase(page_addr);
	if(status == flash_ok) {
		status = flash_write(page_addr, write_data);
		if((FLASH->SR & FLASH_SR_PGAERR))
			usart_put_str(&usart1, "PGAERR\r");
		if((FLASH->SR & FLASH_SR_PGSERR))
			usart_put_str(&usart1, "PGSERR\r");
		if(FLASH->SR & FLASH_SR_PGPERR)
			usart_put_str(&usart1, "PGPERR\r");
		if((FLASH->SR & FLASH_SR_WRPERR))
			usart_put_str(&usart1, "WRPERR\r");
		usart_put_uint32(&usart1, write_data);
	}
	flash_lock();
	/*flash_put_status(status);*/
	/********************write***********************/

	/********************read************************/

	status = flash_read(page_addr, &read_data);

	if(status == flash_ok)
		usart_put_uint32(&usart1, read_data);

	flash_put_status(status);
	/********************read************************/
	
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
