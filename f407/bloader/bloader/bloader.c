#include "bloader.h"
#include "flash.h"

static handle_st_t bloader_erase();
static handle_st_t bloader_unlock();
static handle_st_t bloader_lock();
static handle_st_t bloader_write(const uint32_t *data, const uint32_t size);
static handle_st_t bloader_read(uint32_t *data, const uint32_t size);
static handle_st_t bloader_test(uint32_t *data);

handle_st_t bloader_handle(const dec_buff_t *dec_buff_st, ans_buff_t *ans_buff_st)
{
	handle_st_t handle_st;

	const uint32_t *dec_cnt = &(dec_buff_st->cnt);
	const buff_size_t *dec_buff = (dec_buff_st->buff);

	uint32_t *ans_cnt = &(ans_buff_st->cnt);
	buff_size_t *ans_buff = (ans_buff_st->buff);

	uint32_t size;
	uint32_t *data;

	uint8_t cmd = dec_buff[bl_cmd_pos];
	switch(cmd) {
	case bl_clk_cmd:	usart_put_uint32(&usart1, rcc_get_system_clk() / 1000000);
						handle_st = handle_st_res;
						break;

	case bl_unlock_cmd:	handle_st = bloader_unlock();
						break;

	case bl_lock_cmd:	handle_st = bloader_lock();
						break;

	case bl_erase_cmd:	handle_st = bloader_erase();
						break;

	case bl_write_cmd:	//size = *dec_cnt;
						size = 64;
						data = (uint32_t *)dec_buff;
						handle_st = bloader_write(data, size);
						break;

	case bl_read_cmd:	size = 64;
						data = (uint32_t *)ans_buff;
						*ans_cnt = size * 4;
						handle_st = bloader_read(data, size);
						break;

	case bl_test_cmd:	data = (uint32_t *)ans_buff;
						*ans_cnt = bl_page_size;
						handle_st = bloader_test(data);
						break;

	case bl_main_cmd:	handle_st = handle_st_main;
						break;
	

	default:			handle_st = handle_st_bad;
	}

	return handle_st;
}

handle_st_t bloader_test(uint32_t *data)
{
	const uint32_t page_addr = flash_sector3_addr;
	const uint32_t page_size = 256; /*bytes*/
	const uint32_t data_len = 64;
	uint32_t *read_data = data;
	flash_status_t status;
	uint32_t i;

	uint32_t write_data[data_len];

	for(i = 0; i < data_len; i++)
		write_data[i] = 0xABCDABCD;

	flash_unlock();
	status = flash_erase(page_addr);
	if((FLASH->SR & FLASH_SR_PGAERR) || (FLASH->SR & FLASH_SR_PGSERR) || (FLASH->SR & FLASH_SR_PGPERR) || (FLASH->SR & FLASH_SR_WRPERR))
		return handle_st_bad;

	if(status == flash_ok) {
		status = flash_write_page(page_addr, write_data, page_size);
		if((FLASH->SR & FLASH_SR_PGAERR))
			usart_put_str(&usart1, "PGAERR\r");
		if((FLASH->SR & FLASH_SR_PGSERR))
			usart_put_str(&usart1, "PGSERR\r");
		if(FLASH->SR & FLASH_SR_PGPERR)
			usart_put_str(&usart1, "PGPERR\r");
		if((FLASH->SR & FLASH_SR_WRPERR))
			usart_put_str(&usart1, "WRPERR\r");

//		return handle_st_bad;
	}

	flash_lock();

	if(status == flash_ok)
		status = flash_read_page(page_addr, read_data, page_size);
	if(status == flash_ok)
		return handle_st_res;

	return handle_st_bad;
#if 0
	/********************erase************************/
	flash_unlock();
	status = flash_erase(page_addr);
	if(status != flash_ok) {
		flash_lock();
		return handle_st_bad;
	}
	/********************erase************************/

	/********************write************************/
	status = flash_write_page(page_addr, write_data, page_size);
	flash_lock();
	if(status != flash_ok) {
		return handle_st_bad;
	}
	/********************write************************/

	/********************read************************/
	status = flash_read_page(page_addr, read_data, page_size);

	if(status != flash_ok) {
		return handle_st_bad;
	}
	/********************read************************/

	return handle_st_res;
#endif
}

handle_st_t bloader_erase()
{
	flash_status_t status = flash_erase(main_sector);
	if(status != flash_ok)
		return handle_st_bad;
	return handle_st_res;
}

handle_st_t bloader_write(const uint32_t *data, const uint32_t size)
{
	uint32_t data32, addr;
	int i;
	handle_st_t handle_st = handle_st_res;
	flash_status_t flash_st;
	if(size != bl_page_size)
		return handle_st_bad;

	for(i = 0; i < (bl_page_size / 4); i++) {
		data32 = data[i];
		addr = main_addr + (i * 4);
		flash_st = flash_write(addr, data32);
		if(flash_st != flash_ok) {
			handle_st = handle_st_bad;
			break;
		}
	}

	return handle_st;
}

handle_st_t bloader_read(uint32_t *data, const uint32_t size)
{
	handle_st_t handle_st = handle_st_res;
	uint32_t data32, addr;
	flash_status_t flash_st;
	int i;
	if(size != bl_page_size)
		return handle_st_bad;

	for(i = 0; i < (bl_page_size / 4); i++) {
		addr = main_sector + i;
		flash_st = flash_read(addr, &data32);
		if(flash_st != flash_ok) {
			handle_st = handle_st_bad;
			break;
		}
		data[i] = data32;
	}
	return handle_st;
}

handle_st_t bloader_unlock()
{
	flash_unlock();
	return handle_st_res;
}

handle_st_t bloader_lock()
{
	flash_lock();
	return handle_st_res;
}

#if 0
void bloader_enter()
#endif
