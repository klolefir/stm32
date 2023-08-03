#ifndef FSM_H_SENTRY
#define FSM_H_SENTRY

void fsm_process();

typedef enum {
	idle_state,
	init_state,
	wait_state,
	receive_state,
	decode_state,
	handle_state,
	respond_state,
	purge_state,
	reset_state,
#if 1
	deinit_state,
	gomain_state
#endif
} fsm_state_t;

typedef enum {
	recv_st_ok,
	recv_st_bad
} recv_st_t;

typedef enum {
	handle_st_ok,
	handle_st_rst,
	handle_st_bad,
	handle_st_main,
	handle_st_res
} handle_st_t;

#endif
