#include "flash.h"

enum {
	first_key 	= 0x45670123,
	second_key 	= 0xCDEF89AB
};


void flash_unlock()
{
	FLASH->KEYR = first_key;
	FLASH->KEYR = second_key;
}

static void flash_erase()
{
}

static void flash_wait_until_free(/*const uint32_t timeout*/) //in ms
{
//	uint32_t counter = 0;
	while(FLASH->SR & FLASH_SR_BSY) {
#if 0
		if(counter >= timeout)
			return flash_timeout_err;
		counter++;
#endif
	}
}
