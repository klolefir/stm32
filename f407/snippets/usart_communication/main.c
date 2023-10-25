#include "rcc.h"
#include "operator.h"

void receive_data();

int main(void)
{
	buff_t buff;
	recv_state_t recv_state;

	__enable_irq();
	rcc_init();
	operator_init();

	while(1) {
		recv_state = operator_receive(&buff);
		if(recv_state == recv_state_ok)
			operator_transmit(&buff);
	}
}

#if 0
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
			timer = usart_ticks + 2;		//20 ms delay
		}

		if(((timer) && (usart_ticks >= timer)) || (req_cnt >= req_buff_len)) {
			if(req_cnt) {
				usart_put_buff(&usart1, req_buff, req_cnt);
				timer = 0;
				req_cnt = 0;
			}
		}
	}
}
#endif
