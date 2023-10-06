#ifndef BLOADER_H_SENTRY
#define BLOADER_H_SENTRY

#include "init.h"
#include "fsm.h"

enum {
	bl_page_size = 256 //bytes
};

enum {
	bl_sig_pos 	= 0,
	bl_cmd_pos 	= bl_sig_pos,
	bl_data_pos = 1
};

#if 1
enum {
	bl_erase_cmd_len	= 1,
	bl_write_cmd_len 	= 1,
	bl_read_cmd_len	 	= 1
};
#endif

enum {
	bl_clk_cmd = 'C',
	bl_test_cmd = 'T',
	bl_lock_cmd = 'L',
	bl_unlock_cmd = 'U',
	bl_write_cmd = 'W',
	bl_read_cmd = 'R',
	bl_main_cmd = 'M',
	//bl_reset_cmd = ',
	bl_erase_cmd = 'E'
} bl_cmd_t;

handle_st_t bloader_handle(const dec_buff_t *dec_buff_st, ans_buff_t *ans_buff_st);

#endif
