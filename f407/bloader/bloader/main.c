#include "main.h"

void run_main();
void system_init();
void system_deinit();
void delay_ms(uint32_t ms);
app_flag_t receive_data(req_buff_t *req_buff_st, ans_buff_t *ans_buff_st);
app_flag_t decode_answer(req_buff_t *req_buff_st, ans_buff_t *ans_buff_st);
void usart_put_uint32(uint32_t data);

int main(void)
{
	system_init();

	req_buff_t req_buff_st;
	req_buff_st.cnt = 0;
	ans_buff_t ans_buff_st;
	ans_buff_st.cnt = 0;

	app_flag_t app_flag;
	while(1) {
		app_flag = receive_data(&req_buff_st, &ans_buff_st);
		if(app_flag == app_flag_up)
			run_main();
	}
}

void run_main()
{
	typedef void(*main_f)(void);
	main_f main_jump;

	rcc_deinit();
	system_deinit();

	__disable_irq();
//	__DSB();
//	__ISB();
	SCB->VTOR = main_addr;

	uint32_t msp = *((volatile uint32_t *)(main_addr));
	__set_MSP(msp);

	uint32_t main_jump_addr = *((uint32_t *)(main_addr + 4));
	main_jump = (main_f)main_jump_addr;
	main_jump();
}

void system_init()
{
	rcc_init();
	enable_irq();
	systick_init(1000);

	gpio_init(&led1_pin);
	usart_init(&usart1);
	tim_init(&tim6);
}

void system_deinit()
{
	RCC->AHB1ENR = 0;
	RCC->APB1ENR = 0;
	RCC->APB2ENR = 0;
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
}

app_flag_t receive_data(req_buff_t *req_buff_st, ans_buff_t *ans_buff_st)
{
	uint32_t *req_cnt = &(req_buff_st->cnt);
	char *req_buff = (req_buff_st->buff);
	uint32_t *ans_cnt = &(ans_buff_st->cnt);
	char *ans_buff = (ans_buff_st->buff);

	static uint32_t timer = 0;

	if((USART1->SR & USART_SR_RXNE)) {
		req_buff[*req_cnt] = USART1->DR;
		(*req_cnt)++;
		timer = usart_ticks + 2;
	}

	app_flag_t app_flag = app_flag_down; 
	if(((timer) && (usart_ticks >= timer)) || (*req_cnt >= req_buff_len)) {
		if(*req_cnt) {
			app_flag = decode_answer(req_buff_st, ans_buff_st);
			if(*ans_cnt)
				usart_put_buff(&usart1, ans_buff, *ans_cnt);
			timer = 0;
			*req_cnt = 0;
		}
	}
	return app_flag;
}

app_flag_t decode_answer(req_buff_t *req_buff_st, ans_buff_t *ans_buff_st)
{
	app_flag_t app_flag = app_flag_down;
	const char go_main_str[] = "Go main...\r";
	const char info_str[] = "I'm  bloader!\r";
	const char no_savvy_str[] = "No savvy @_@\r";

	//uint32_t *req_cnt = &(req_buff_st->cnt);
	char *req_buff = (req_buff_st->buff);
	uint32_t *ans_cnt = &(ans_buff_st->cnt);
	char *ans_buff = (ans_buff_st->buff);

	*ans_cnt = 0;

	char command = req_buff[0];
	switch(command) {
	case 'B': 	app_flag = app_flag_up;
				*ans_cnt = sizeof(go_main_str) - 1;
				memcpy(ans_buff, go_main_str, sizeof(go_main_str));
				break;

	case 'I':	*ans_cnt = sizeof(info_str) - 1;
				memcpy(ans_buff, info_str, sizeof(info_str));
				break;

	default:	*ans_cnt = sizeof(no_savvy_str) - 1;
				memcpy(ans_buff, no_savvy_str, sizeof(no_savvy_str));
				break;
	}

	return app_flag;
}

void usart_put_uint32(uint32_t data)
{
	char data_str[6];
	memcpy(data_str, &data, sizeof(uint32_t));
	data_str[4] = '\r';
	data_str[5] = '\0';
	usart_put_str(&usart1, data_str);
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
