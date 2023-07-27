#ifndef FLASH_H_SENTRY
#define FLASH_H_SENTRY

#include <stm32f4xx.h>

typedef enum {
	flash_timeout_err,
	flash_ok
} flash_status_t;

void flash_unlock();
void flash_write();
void flash_read();

#endif
