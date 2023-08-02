#include "fsm.h"
#include "init.h"

static void init();
static recv_st_t receive(req_buff_t *req_buff_st);
static void decode(const req_buff_t *req_buff_st, dec_buff_t *dec_buff_st);
static handle_st_t handle(const dec_buff_t *dec_buff_st, ans_buff_t *ans_buff_st);
static void respond(ans_buff_t *ans_buff_st);
static void purge(req_buff_t *req_buff_st);
static void reset();

void fsm_process()
{
	req_buff_t req_buff_st;
	req_buff_st.cnt = 0;

	dec_buff_t dec_buff_st;
	dec_buff_st.cnt = 0;

	ans_buff_t ans_buff_st;
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
								if(recv_st == recv_st_ok)
									state = decode_state;
								else
									state = receive_state;
								break;

		case decode_state:		decode(&req_buff_st, &dec_buff_st);
								state = handle_state;
								break;

		case handle_state:		handle_st = handle(&dec_buff_st, &ans_buff_st);
								state = respond_state;
								break;

		case respond_state:		respond(&ans_buff_st);
								if(handle_st == handle_st_rst)
									state = reset_state;
								else
									state = purge_state;
								break;

		case purge_state:		purge(&req_buff_st);
								state = receive_state;
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
	char *req_buff = (req_buff_st->buff);

	static uint32_t timer = 0;
	uint32_t usart_ticks;

	if((USART1->SR & USART_SR_RXNE)) {
		req_buff[*req_cnt] = USART1->DR;
		(*req_cnt)++;
		usart_ticks = tim_get_ticks(&usart_tim);
		timer = usart_ticks + 2;
	}

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
	const char *req_buff = (req_buff_st->buff);

	uint32_t *dec_cnt = &(dec_buff_st->cnt);
	char *dec_buff = (dec_buff_st->buff);

	uint32_t i;
	for(i = 0; i < *req_cnt; i++)
		dec_buff[i] = req_buff[i];

	*dec_cnt = *req_cnt;
}

handle_st_t handle(const dec_buff_t *dec_buff_st, ans_buff_t *ans_buff_st)
{
	const char reset_str[] = "Reset...\r";
	const char info_str[] = "I'm  main!\r";
	const char no_savvy_str[] = "No savvy @_@\r";
	const char *ans_str;

	const uint32_t *dec_cnt = &(dec_buff_st->cnt);
	const char *dec_buff = (dec_buff_st->buff);

	uint32_t *ans_cnt = &(ans_buff_st->cnt);
	char *ans_buff = (ans_buff_st->buff);

	handle_st_t handle_st = handle_st_ok;

	if(!(*dec_cnt))
		return handle_st_bad;

	char command = dec_buff[0];
	switch(command) {
	case 'R': 	handle_st = handle_st_rst;	
				ans_str = reset_str;	
				break;

	case 'I':	ans_str = info_str;	
				break;

	default:	ans_str = no_savvy_str;
				break;
	}

	*ans_cnt = kestrlen(ans_str);
	kememcpy(ans_buff, ans_str, *ans_cnt);

	return handle_st;
}

void respond(ans_buff_t *ans_buff_st)
{
	uint32_t *ans_cnt = &(ans_buff_st->cnt);
	char *ans_buff = (ans_buff_st->buff);
	
	if(*ans_cnt)
		usart_put_buff(&usart1, ans_buff, *ans_cnt);
}

void purge(req_buff_t *req_buff_st)
{
	uint32_t *req_cnt = &(req_buff_st->cnt);
	*req_cnt = 0;
}
