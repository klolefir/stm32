#ifndef FLASH_H_SENTRY
#define FLASH_H_SENTRY

#include <stm32f4xx.h>

typedef enum {
	flash_bad_addr_err,
	flash_lock_err,
	flash_timeout_err,
	flash_ok
} flash_status_t;

typedef enum {
	flash_locked,
	flash_unlocked
} flash_lock_status_t;

enum {
	first_key 	= 0x45670123,
	second_key	= 0xCDEF89AB
};

/*
 * 	@brief	Small sectors: 	0, 1, 2, 3
 * 			Middle sectors:	4
 * 			Large sectors:	5, 6, 7, 8, 9, 10, 11
 */

enum {
	flash_addr_offset = 0x04
};

enum {
	flash_start_addr		= 0x08000000,

	flash_sector0_addr		= 0x08000000,	//small
	flash_sector1_addr		= 0x08004000,
	flash_sector2_addr		= 0x08008000,
	flash_sector3_addr		= 0x0800C000,

	flash_sector4_addr		= 0x08010000,	//middle

	flash_sector5_addr		= 0x08020000,	//large
	flash_sector6_addr		= 0x08040000,	
	flash_sector7_addr		= 0x08060000,
	flash_sector8_addr		= 0x08080000,
	flash_sector9_addr		= 0x080A0000,
	flash_sector10_addr		= 0x080C0000,
	flash_sector11_addr		= 0x080E0000,

	flash_main_mem_end_addr	= 0x080FFFFF,

	flash_system_mem_addr	= 0x1FFF0000,
	flash_otp_addr			= 0x1FFF7800,
	flash_opt_bytes_addr	= 0x1FFFC000,

	flash_end_addr			= 0x1FFFC00F,

	small_sector_size		= flash_sector1_addr - flash_sector0_addr,
	middle_sector_size		= flash_sector5_addr - flash_sector4_addr,
	large_sector_size		= flash_sector6_addr - flash_sector5_addr
};

void flash_unlock();
void flash_lock();
flash_status_t flash_write(uint32_t data_addr, uint32_t data);
flash_status_t flash_write_page(uint32_t page_addr, uint32_t *data, uint32_t page_size);
flash_status_t flash_read(uint32_t data_addr, uint32_t *data);
flash_status_t flash_read_page(uint32_t page_addr, uint32_t *data, uint32_t page_size);
flash_status_t flash_erase(uint32_t sector_addr);

#endif
