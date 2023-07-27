#include "main.h"

//void delay_ms(uint32_t ms);
void receive_data();

int main(void)
{
	rcc_init();
	enable_irq();
	//systick_init(1000);

	gpio_init(&led1_pin);
	usart_init(&usart1);
	tim_init(&tim6);
	
	receive_data();
	while(1) {
	}
}

#if 0
void delay_ms(uint32_t ms)
{
	uint32_t start = ticks;
	uint32_t end = start + ms;

	if(end < start)
		while(ticks > start) {}

	while(ticks < end) {}
}
#endif

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

#if 0
void systick_handler()
{
	ticks++;
}
#endif

#if 0
void usart1_handler()
{
	char c = USART1->DR;
	usart_put_char(&usart1, 'h');
	GPIOA->ODR ^= (1 << LED1_PIN);
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
