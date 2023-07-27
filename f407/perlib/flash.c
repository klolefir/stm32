#include "flash.h"
#include "general.h"

static void wait_until_bsy();
static flash_status_t flash_erase(uint32_t page_addr);
static flash_lock_status_t flash_check_lock();
static flash_status_t flash_switch_sector(uint32_t page_addr, uint8_t *mask);

void flash_unlock()
{
	wait_until_bsy();
	flash_lock_status_t status;
	status = flash_check_lock();
	if(status == flash_locked) {
		set_reg(&FLASH->KEYR, first_key);
		set_reg(&FLASH->KEYR, second_key);
	}
}

void flash_lock()
{
	wait_until_bsy();
	flash_lock_status_t status;
	status = flash_check_lock();
	if(status == flash_unlocked)
		set_bit(&FLASH->CR, FLASH_CR_LOCK);
}

flash_status_t flash_write(uint32_t page_addr, const uint32_t data)
{
	flash_lock_status_t lock_status = flash_check_lock();
	if(lock_status == flash_locked)
		return flash_lock_err;

	flash_status_t status = flash_erase(page_addr);

	/*debug*/
	wait_until_bsy();
	if(FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}
	/*debug*/

	if(status != flash_ok)
		return status;
	wait_until_bsy();

	set_bit(&FLASH->CR, FLASH_CR_PG);		//enable flash programming
	set_bit(&FLASH->CR, FLASH_CR_PSIZE_1);	//word access
	*(uint32_t *)page_addr = data;
	while(!(FLASH->SR & FLASH_SR_EOP)) {}
	FLASH->SR = FLASH_SR_EOP;


	return flash_ok;
}

flash_status_t flash_read(uint32_t page_addr, uint32_t *data)
{
	flash_lock_status_t lock_status = flash_check_lock();
	if(lock_status == flash_locked)
		return flash_lock_err;

	wait_until_bsy();
	*data = *(uint32_t *)page_addr;
	return flash_ok;
}

flash_status_t flash_erase(uint32_t page_addr)
{
	wait_until_bsy();
	/*debug*/
	if(FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}
	/*debug*/

	uint8_t sector_mask;

	set_bit(&FLASH->CR, FLASH_CR_SER);				//enable sector erase

	flash_status_t status = flash_switch_sector(page_addr, &sector_mask);
	if(status != flash_ok)
		return status;

	set_bit(&FLASH->CR, sector_mask);				//set sector
	set_bit(&FLASH->CR, FLASH_CR_STRT);				//start erase
	/*debug*/
	while(!(FLASH->SR & FLASH_SR_EOP)) {}
	/*debug*/
	return flash_ok;
}

void wait_until_bsy()
{
	uint8_t is_bsy;
	do {
		is_bsy = read_bit(&FLASH->SR, FLASH_SR_BSY);
	} while(is_bsy);
}

flash_status_t flash_switch_sector(uint32_t page_addr, uint8_t *mask)
{
	if((page_addr >= flash_sector0_addr) && (page_addr < flash_sector1_addr)) 
		*mask = FLASH_CR_SNB_0;										//sector 0
	else if((page_addr >= flash_sector1_addr) && (page_addr < flash_sector2_addr))
		*mask = FLASH_CR_SNB_1;										//sector 1
	else if((page_addr >= flash_sector2_addr) && (page_addr < flash_sector3_addr))
		*mask = FLASH_CR_SNB_0 | FLASH_CR_SNB_1; 					//sector 2
	else if((page_addr >= flash_sector3_addr) && (page_addr < flash_sector4_addr))
		*mask = FLASH_CR_SNB_2;										//sector 3
	else if((page_addr >= flash_sector4_addr) && (page_addr < flash_sector5_addr))
		*mask = FLASH_CR_SNB_0 | FLASH_CR_SNB_2;					//sector 4
	else if((page_addr >= flash_sector5_addr) && (page_addr < flash_sector6_addr))
		*mask = FLASH_CR_SNB_1 | FLASH_CR_SNB_2;					//sector 5
	else if((page_addr >= flash_sector6_addr) && (page_addr < flash_sector7_addr))
		*mask = FLASH_CR_SNB_0 | FLASH_CR_SNB_1 | FLASH_CR_SNB_2;	//sector 6
	else if((page_addr >= flash_sector7_addr) && (page_addr < flash_sector8_addr))
		*mask = FLASH_CR_SNB_3;										//sector 7
	else if((page_addr >= flash_sector8_addr) && (page_addr < flash_sector9_addr))
		*mask = FLASH_CR_SNB_0 | FLASH_CR_SNB_3;					//sector 8
	else if((page_addr >= flash_sector9_addr) && (page_addr < flash_sector10_addr))
		*mask = FLASH_CR_SNB_1 | FLASH_CR_SNB_3;					//sector 9
	else if((page_addr >= flash_sector10_addr) && (page_addr < flash_sector11_addr))
		*mask = FLASH_CR_SNB_2 | FLASH_CR_SNB_3;					//sector 10
	else if((page_addr >= flash_sector11_addr) && (page_addr <= flash_main_mem_end_addr))
		*mask = FLASH_CR_SNB_0 | FLASH_CR_SNB_2 | FLASH_CR_SNB_3;	//sector 11
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
