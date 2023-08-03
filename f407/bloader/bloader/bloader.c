#include "bloader.h"
#include "init.h"

handle_st_t bloader_handle(const req_buff_t *req_buff_st, ans_buff_t *ans_buff_st)
{
	handle_st_t handle_st;

	uint32_t *req_cnt = &(req_buff_st->cnt);
	char *req_buff = (req_buff_st->buff);

	uint8_t cmd = req_buff[bl_cmd_pos];
	switch(cmd) {
	case bl_erase_cmd:	handle_st = bloader_erase();
						break;

	case bl_write_cmd:	uint32_t size = *req_cnt - bl_write_cmd_len;
						char *data = req_buff[bl_data_pos];
						handle_st = bloader_write(req_buff[bl_data_pos], size);
						break;

	case bl_read_cmd:	handle_st = bloader_read();
						break;

	default:			handle_st = handle_st_bad;
	}



	return handle_st;
}

handle_st_t bloader_erase()
{
	flash_status_t status = flash_erase(main_sector);
	if(status != flash_ok)
		return handle_st_bad;		
	return handle_st_res;
}

handle_st_t bloader_write(const uint8_t *data, const uint32_t size)
{
	uint32_t *data32;
	int i;
	handle_st_t handle_st = handle_st_res;
	flash_st_t flash_st;
	if(size != bl_page_size)
		return handle_st_bad;

	for(i = 0; i < bl_page_size; i++) {
		data32 = (uint32_t *)data[i];
		flash_st = flash_write(main_addr, *data32);
		if(flash_st != flash_ok) {
			handle_st = handle_st_bad;	
			break;
		}
	}

	return handle_st;
}

handle_st_t bloader_read(uint8_t *data

#if 0
void bloader_enter()
#endif
