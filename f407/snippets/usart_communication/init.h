#ifndef INIT_H_SENTRY
#define INIT_H_SENTRY

#include <stdint.h>

enum {
	max_buff_len = 1024
};

typedef uint8_t buff_size_t;

typedef struct {
	uint8_t buff[max_buff_len];
	uint32_t cnt;
} buff_t;

#endif
