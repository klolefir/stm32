#include "fsm.h"
#include "init.h"
#include "gpio.h"
#include "rcc.h"
#include "general.h"
#include "systick.h"
#include "usart.h"
#include "tim.h"
#include "bloader.h"
#include "kestring.h"

static void init();
static recv_st_t receive(req_buff_t *req_buff_st);
static void decode(const req_buff_t *req_buff_st, dec_buff_t *dec_buff_st);
static handle_st_t handle(const dec_buff_t *dec_buff_st, ans_buff_t *ans_buff_st);
static void respond(const ans_buff_t *ans_buff_st);
static void purge(req_buff_t *req_buff_st, ans_buff_t *ans_buff_st);
static void reset();
static void deinit();
static void gomain();
static void error(ans_buff_t *ans_buff_st);

fsm_state_t switch_state_after_recv(const recv_st_t recv_status);
fsm_state_t switch_state_after_handle(const handle_st_t handle_status);

req_buff_t req_buff_st;
dec_buff_t dec_buff_st;
ans_buff_t ans_buff_st;

void fsm_process()
{
	//req_buff_t req_buff_st;
	req_buff_st.cnt = 0;

	//dec_buff_t dec_buff_st;
	dec_buff_st.cnt = 0;

	//ans_buff_t ans_buff_st;
	ans_buff_st.cnt = 0;

	recv_st_t 	recv_st;
	handle_st_t handle_st;

	fsm_state_t state = idle_state;
	while(1) {
		switch(state) {

		case idle_state:		state = init_state;
								break;

		case init_state:		init();
								state = wait_state;
								break;

		case wait_state:		state = receive_state;
								break;

		case receive_state:		recv_st = receive(&req_buff_st);
								state = switch_state_after_recv(recv_st);
								break;

		case decode_state:		decode(&req_buff_st, &dec_buff_st);
								state = handle_state;
								break;

		case handle_state:		handle_st = handle(&dec_buff_st, &ans_buff_st);
								state = switch_state_after_handle(handle_st);
								break;

		case error_state:		error(&ans_buff_st);
								state = respond_state;
								break;

		case respond_state:		respond(&ans_buff_st);
								state = purge_state;
								break;

		case purge_state:		purge(&req_buff_st, &ans_buff_st);
								state = receive_state;
								break;

		case deinit_state:		deinit();
								state = gomain_state;
								break;

		case gomain_state:		gomain();
								break;

		case reset_state:		reset();
								break;

		}
	}
}

void reset()
{
	NVIC_SystemReset();
}

void init()
{
	rcc_init();
	gpio_init(&led1_pin);
	systick_init(1000);
	usart_init(&usart1);
	tim_init(&usart_tim);
}

recv_st_t receive(req_buff_t *req_buff_st)
{
	recv_st_t recv_st = recv_st_bad;

	uint32_t *req_cnt = &(req_buff_st->cnt);
	buff_size_t *req_buff = (req_buff_st->buff);

	static uint32_t timer = 0;
	uint32_t usart_ticks;// = 0;
	char c;

	usart_rx_status_t rx_status = usart_get_rx_status(&usart1);
	if(rx_status == usart_rx_rdy) {
		usart_get_char(&usart1, &c);
		req_buff[*req_cnt] = c;
		(*req_cnt)++;
		usart_ticks = tim_get_ticks(&usart_tim);
		timer = usart_ticks + 2;
	}

	//usart_ticks = tim_get_ticks(&usart_tim);
	if(((timer) && (usart_ticks >= timer) && (*req_cnt)) ||
		(*req_cnt >= req_buff_len))
	{
		recv_st = recv_st_ok;
		timer = 0;
	}

	return recv_st;
}

void decode(const req_buff_t *req_buff_st, dec_buff_t *dec_buff_st)
{
	const uint32_t *req_cnt = &(req_buff_st->cnt);
	const buff_size_t *req_buff = (req_buff_st->buff);

	uint32_t *dec_cnt = &(dec_buff_st->cnt);
	buff_size_t *dec_buff = (dec_buff_st->buff);

	kememcpy(dec_buff, req_buff, *req_cnt);
	*dec_cnt = *req_cnt;
}

handle_st_t handle(const dec_buff_t *dec_buff_st, ans_buff_t *ans_buff_st)
{
#if 0
	const char ok_char = 'O';
	const char bad_char = 'B';
	const char none_char = 'N';
	char ans_char;
	const char reset_str[] = "Reset...\r";
	const char gomain_str[] = "Gomain...\r";
	const char info_str[] = "I'm  bloader!\r";
	const char no_savvy_str[] = "No savvy @_@\r";
	const char *ans_str;

	uint32_t *ans_cnt = &(ans_buff_st->cnt);
	buff_size_t *ans_buff = (ans_buff_st->buff);


	switch(handle_st) {
	case handle_st_bad:	ans_char = bad_char;
						break;

	case handle_st_res:	ans_char = ok_char;
						break;

	default:			ans_char = none_char;
	}

	if(handle_st != handle_st_main)
		handle_st = handle_st_res;

	*ans_cnt += sizeof(char);
	ans_buff[0] = ans_char;
	//ans_buff[*ans_cnt] = '\0';
#endif

	handle_st_t handle_st = bloader_handle(dec_buff_st, ans_buff_st);
	return handle_st;
#if 0
	const uint32_t *dec_cnt = &(dec_buff_st->cnt);
	const char *dec_buff = (dec_buff_st->buff);

	uint32_t *ans_cnt = &(ans_buff_st->cnt);
	char *ans_buff = (ans_buff_st->buff);

	handle_st_t handle_st = handle_st_res;

	if(!(*dec_cnt))
		return handle_st_bad;

	char command = dec_buff[0]; switch(command) {
	case 'R': 	handle_st = handle_st_rst;	
				ans_str = reset_str;	
				break;

	case 'M': 	handle_st = handle_st_main;	
				ans_str = gomain_str;
				break;

	case 'I':	ans_str = info_str;	
				break;

	default:	ans_str = no_savvy_str;
	}

	*ans_cnt = kestrlen(ans_str);
	kememcpy(ans_buff, ans_str, *ans_cnt);

	return handle_st;
#endif
}

void error(ans_buff_t *ans_buff_st)
{
	//err_t err = get_err()
	//switch(err) { ... }
	uint32_t *ans_cnt = &(ans_buff_st->cnt);
	buff_size_t *ans_buff = (ans_buff_st->buff);

	const char err_str[] = "Error!\r";	
	*ans_cnt = kestrlen(err_str);
	kememcpy(ans_buff, err_str, *ans_cnt);
}

void respond(const ans_buff_t *ans_buff_st)
{
	const uint32_t ans_cnt = ans_buff_st->cnt;
	const buff_size_t *ans_buff = (ans_buff_st->buff);
	
	if(ans_cnt)
		usart_put_buff(&usart1, ans_buff, ans_cnt);
}

void purge(req_buff_t *req_buff_st, ans_buff_t *ans_buff_st)
{
	uint32_t *req_cnt = &(req_buff_st->cnt);
	uint32_t *ans_cnt = &(ans_buff_st->cnt);
	*req_cnt = 0;
	*ans_cnt = 0;
}

#if 1
void deinit()
{
	rcc_deinit();
	RCC->AHB1ENR = 0;
	RCC->APB1ENR = 0;
	RCC->APB2ENR = 0;
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
}

void gomain()
{
	typedef void(*main_f)(void);
	main_f main_jump;

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
#endif

fsm_state_t switch_state_after_recv(const recv_st_t recv_st)
{
	fsm_state_t next_state;
	switch(recv_st) {
	case recv_st_ok:	next_state = decode_state;
						break;
	case recv_st_bad:	next_state = receive_state;
						break;
	default:			next_state = purge_state;	
	}
	return next_state;
}

fsm_state_t switch_state_after_handle(const handle_st_t handle_st)
{
	fsm_state_t next_state;
	switch(handle_st) {
	case handle_st_rst:		next_state = reset_state;
							break;

	case handle_st_main:	next_state = deinit_state;
							break;

	case handle_st_res:		next_state = respond_state;
							break;

	case handle_st_bad:		next_state = error_state;
							break;

	default:				next_state = purge_state;
							break;
	}
	return next_state;
}

