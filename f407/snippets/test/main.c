#include "main.h"
#include "string.h"

void delay_ms(uint32_t ms);
void receive_data();

const uint32_t page_addr = flash_sector8_addr;
const uint32_t write_data = 0xDEADBEEF;

void flash_enable_irq()
{
	flash_unlock();
	set_bit(&FLASH->CR, FLASH_CR_ERRIE);
	set_bit(&FLASH->CR, FLASH_CR_EOPIE);
	flash_lock();
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

void flash_write_test()
{
	flash_unlock();
	char data_str[5];
	memcpy(data_str, &write_data, sizeof(uint32_t));
	data_str[4] = '\0';
	flash_status_t status = flash_write(page_addr, write_data);
	usart_put_str(&usart1, "Write data: ");
	usart_put_str(&usart1, data_str);
	usart_put_char(&usart1, '\r');
	flash_put_status(status);
	flash_lock();
}

void flash_read_test()
{
	uint32_t read_data;
	char data_str[5];
	flash_unlock();
	flash_status_t status = flash_read(page_addr, &read_data);
	memcpy(data_str, &read_data, sizeof(uint32_t));
	data_str[4] = '\0';
	flash_put_status(status);
	flash_lock();

	if(read_data == write_data)
		usart_put_str(&usart1, "Ok data: ");
	else
		usart_put_str(&usart1, "Bad data: ");

	usart_put_str(&usart1, data_str);
	usart_put_char(&usart1, '\r');
}

int main(void)
{
	rcc_init();
	enable_irq();
	//systick_init(1000);

	gpio_init(&led1_pin);
	usart_init(&usart1);
	//tim_init(&tim6);
	
	NVIC_EnableIRQ(FLASH_IRQn);
	NVIC_SetPriority(FLASH_IRQn, 0);
	//NVIC_EnableIRQ(6);
	//NVIC_SetPriority(6, 0);

	flash_enable_irq();
	flash_write_test();
	flash_read_test();
	while(1) {
	}
}

void flash_handler()
{
	usart_put_str(&usart1, "In handler!\r");
	uint8_t is_eop = read_bit(&FLASH->SR, FLASH_SR_EOP);
	if(is_eop) {
		usart_put_str(&usart1, "Handler: success operation\r");
	} else {
		uint8_t is_pgserr = read_bit(&FLASH->SR, FLASH_SR_PGSERR);
		if(is_pgserr)
			usart_put_str(&usart1, "Handler: pgserr\r");
		uint8_t is_pgperr = read_bit(&FLASH->SR, FLASH_SR_PGPERR);
		if(is_pgperr)
			usart_put_str(&usart1, "Handler: pgsperr\r");
		uint8_t is_pgaerr = read_bit(&FLASH->SR, FLASH_SR_PGAERR);
		if(is_pgaerr)
			usart_put_str(&usart1, "Handler: pgaerr\r");
		uint8_t is_wrperr = read_bit(&FLASH->SR, FLASH_SR_WRPERR);
		if(is_wrperr)
			usart_put_str(&usart1, "Handler: wrperr\r");
	}
}







void receive_data()
{
	const uint32_t req_buff_len = 1024;
	uint32_t req_cnt = 0;
	char req_buff[req_buff_len];
	uint32_t timer = 0;
	while(1) {
		if((USART1->SR & USART_SR_RXNE)) {
			req_buff[req_cnt] = USART1->DR;
			req_cnt++;
			timer = usart_ticks + 2;
		}

		if(((timer) && (usart_ticks >= timer)) || (req_cnt >= req_buff_len)) {
			if(req_cnt) {
				GPIOA->ODR ^= (1 << LED1_PIN);
				usart_put_buff(&usart1, req_buff, req_cnt);
				timer = 0;
				req_cnt = 0;
			}
		}
	}
}

#if 1
void delay_ms(uint32_t ms)
{
	uint32_t start = ticks;
	uint32_t end = start + ms;

	if(end < start)
		while(ticks > start) {}

	while(ticks < end) {}
}
#endif

#if 1
void systick_handler()
{
	ticks++;
}
#endif

void tim6_dac_handler()
{
	uint8_t is_tim6 = read_bit(&TIM6->SR, TIM_SR_UIF);
	if(is_tim6) {
		clear_bit(&TIM6->SR, TIM_SR_UIF);
		usart_ticks++;
	}
}
