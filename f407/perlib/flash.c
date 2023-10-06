#include "flash.h"
#include "general.h"


static flash_lock_status_t flash_check_lock();
static void flash_wait_until_bsy();
static flash_status_t flash_switch_sector(const uint32_t page_addr, uint32_t *mask);

flash_status_t flash_write(/*const*/ uint32_t data_addr, /*const*/ uint32_t data)
{
	while(FLASH->SR & FLASH_SR_BSY) {}
	FLASH->CR |= FLASH_CR_PSIZE_1;
	FLASH->CR |= FLASH_CR_PG;
	while(FLASH->SR & FLASH_SR_BSY) {}
	*(uint32_t *)data_addr = data;
	while(FLASH->SR & FLASH_SR_BSY) {}
	FLASH->CR &= ~FLASH_CR_PG;
	return flash_ok;
}

flash_status_t flash_read(const uint32_t data_addr, uint32_t *data)
{
	flash_unlock();

	flash_wait_until_bsy();
	*data = *(uint32_t *)data_addr;
	flash_wait_until_bsy();

	flash_lock();

	return flash_ok;
}

flash_status_t flash_erase(const uint32_t sector_addr)
{
	uint32_t sector_mask;
	flash_status_t status = flash_switch_sector(sector_addr, &sector_mask);
	if(status != flash_ok)
		return status;

	while(FLASH->SR & FLASH_SR_BSY) {}

	FLASH->CR |= FLASH_CR_SER;
	FLASH->CR |= sector_mask;
	FLASH->CR |= FLASH_CR_STRT;

	while(FLASH->SR & FLASH_SR_BSY) {}

	FLASH->CR &= ~FLASH_CR_SER;

	return flash_ok;
}

flash_status_t flash_write_page(/*const*/ uint32_t page_addr, /*const*/ uint32_t *data, /*const*/ uint32_t page_size)
{
	/*const*/ uint32_t *write_data;
	/*const*/ uint32_t data_size = sizeof(uint32_t);
	uint32_t addr;
	flash_status_t status;
	int i;

	/********************write***********************/
	for(i = 0; i < (page_size / data_size); i++) {
		write_data = &data[i];
		addr = page_addr + (i * flash_addr_offset);
		status = flash_write(addr, *write_data);
		if(status != flash_ok)
			break;
	}
	/********************write***********************/

	return status;
}

flash_status_t flash_read_page(/*const*/ uint32_t page_addr, uint32_t *data, /*const*/ uint32_t page_size)
{
	uint32_t *read_data;
	/*const*/ uint32_t data_size = sizeof(uint32_t);
	uint32_t addr;

	flash_status_t status;
	int i;
	/********************read************************/
	for(i = 0; i < (page_size / data_size); i++) {
		read_data = &data[i];
		addr = page_addr + (i * flash_addr_offset);
		status = flash_read(addr, read_data);
		if(status != flash_ok)
			break;;
	}
	/********************read************************/

	return status;
}

void flash_unlock()
{
	flash_wait_until_bsy();
	flash_lock_status_t status;
	status = flash_check_lock();
	if(status == flash_locked) {
		set_reg(&FLASH->KEYR, first_key);
		set_reg(&FLASH->KEYR, second_key);
	}
}

void flash_lock()
{
	flash_wait_until_bsy();
	flash_lock_status_t status;
	status = flash_check_lock();
	if(status == flash_unlocked)
		FLASH->CR |= FLASH_CR_LOCK;
}


void flash_wait_until_bsy()
{
#if 0
	uint8_t is_bsy;
	do {
		is_bsy = read_bit(&FLASH->SR, FLASH_SR_BSY);
	} while(is_bsy);
#endif
	while((FLASH->SR & FLASH_SR_BSY)) {}
}

flash_status_t flash_switch_sector(const uint32_t page_addr, uint32_t *mask)
{
	if((page_addr >= flash_sector0_addr) && (page_addr < flash_sector1_addr)) 
		*mask = 0;
	else if((page_addr >= flash_sector1_addr) && (page_addr < flash_sector2_addr))
		*mask = FLASH_CR_SNB_0;										//sector 1
	else if((page_addr >= flash_sector2_addr) && (page_addr < flash_sector3_addr))
		*mask = FLASH_CR_SNB_1;										//sector 2
	else if((page_addr >= flash_sector3_addr) && (page_addr < flash_sector4_addr))
		*mask = FLASH_CR_SNB_0 | FLASH_CR_SNB_1; 					//sector 3
	else if((page_addr >= flash_sector4_addr) && (page_addr < flash_sector5_addr))
		*mask = FLASH_CR_SNB_2;										//sector 4
	else if((page_addr >= flash_sector5_addr) && (page_addr < flash_sector6_addr))
		*mask = FLASH_CR_SNB_0 | FLASH_CR_SNB_2;					//sector 5
	else if((page_addr >= flash_sector6_addr) && (page_addr < flash_sector7_addr))
		*mask = FLASH_CR_SNB_1 | FLASH_CR_SNB_2;					//sector 6
	else if((page_addr >= flash_sector7_addr) && (page_addr < flash_sector8_addr))
		*mask = FLASH_CR_SNB_0 | FLASH_CR_SNB_1 | FLASH_CR_SNB_2;	//sector 7
	else if((page_addr >= flash_sector8_addr) && (page_addr < flash_sector9_addr))
		*mask = FLASH_CR_SNB_3;										//sector 8
	else if((page_addr >= flash_sector9_addr) && (page_addr < flash_sector10_addr))
		*mask = FLASH_CR_SNB_0 | FLASH_CR_SNB_3;					//sector 9
	else if((page_addr >= flash_sector10_addr) && (page_addr < flash_sector11_addr))
		*mask = FLASH_CR_SNB_1 | FLASH_CR_SNB_3;					//sector 10
	else if((page_addr >= flash_sector11_addr) && (page_addr <= flash_main_mem_end_addr))
		*mask = FLASH_CR_SNB_0 | FLASH_CR_SNB_1 | FLASH_CR_SNB_3;	//sector 11
	else
		return flash_bad_addr_err;

	return flash_ok;
}

flash_lock_status_t flash_check_lock()
{
	uint8_t is_lock = read_bit(&FLASH->SR, FLASH_CR_LOCK);
	if(is_lock)
		return flash_locked;
	else
		return flash_unlocked;
}
