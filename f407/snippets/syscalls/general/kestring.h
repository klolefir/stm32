#ifndef KESTRING_H_SENTRY
#define KESTRING_H_SENTRY

#include <stm32f4xx.h>

uint32_t kestrlen(const char *str);
void kememcpy(void *dst, const void *src, const uint32_t count);

#endif
