#include "kestring.h"

uint32_t kestrlen(const char *str)
{
	uint32_t len = 0;
	while(*str) {
		len++;
		str++;
	}
	return len;
}

void kememcpy(void *dst, const void *src, const uint32_t count)
{
	/*byte access*/
	uint8_t *dst_byte = dst;
	const uint8_t *src_byte = src;
	uint32_t i;

	for(i = 0; i < count; i++) {
		*dst_byte = *src_byte;
		dst_byte++;
		src_byte++;
	}
}
