#include "main.h"

void delay_ms(uint32_t ms)
{
	uint32_t start = ticks;
	uint32_t end = start + ms;

	if(end < start)
		while(ticks > start) {}

	while(ticks < end) {}
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

int main(void)
{
	rcc_init();
	enable_irq();
	systick_init(1000);

	gpio_init(&led1_pin);
	usart_init(&usart1);
	tim_init(&tim6);
	
	receive_data();
	while(1) {
#if 0
		if((USART1->SR & USART_SR_RXNE)) {
			char c = USART1->DR;
			usart_put_char(&usart1, 'x');
		}
#endif
	}
}

void systick_handler()
{
	ticks++;
}

void usart1_handler()
{
	char c = USART1->DR;
	usart_put_char(&usart1, 'h');
	GPIOA->ODR ^= (1 << LED1_PIN);
}

void tim6_dac_handler()
{
	if(TIM6->SR & TIM_SR_UIF) {
		//GPIOA->ODR ^= (1 << LED1_PIN);
		TIM6->SR &= ~TIM_SR_UIF;
//		usart_put_char(&usart1, 'a');
		usart_ticks++;
	}
}
