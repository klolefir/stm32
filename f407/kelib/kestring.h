#ifndef KESTRING_H_SENTRY
#define KESTRING_H_SENTRY

#include <stdint.h>

uint32_t kestrlen(const char *str);
void kememcpy(void *dst, const void *src, const uint32_t count);
void kelmemcpy(void *dst, const void *src, const uint32_t count);

#endif
