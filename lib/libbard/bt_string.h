#ifndef _BT_STRING_H
#define _BT_STRING_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

typedef struct {
  uint32_t size;
  uint8_t *buf;
} btstring_t;

void bts_free(const void *instr);
btstring_t *bts_new(uint32_t size);
btstring_t *bts_copy(btstring_t *in);
btstring_t *bts_ncopy(btstring_t *in, uint32_t len, uint32_t offset);
btstring_t *bts_strncpy(const uint8_t *str, uint32_t len);
btstring_t *bts_strcpy(const uint8_t *str);
btstring_t *bts_strncat(btstring_t *dest, const uint8_t *str, uint32_t len);
btstring_t *bts_strcat(btstring_t *dest, const uint8_t *str);
btstring_t *bts_sprintf(uint8_t *format, ...);
btstring_t *bts_vsprintf(uint8_t *format, va_list args);
btstring_t *bts_resize(btstring_t *in, uint32_t size);
void bts_memset(btstring_t *t, int c);
size_t		bts_hashcode(btstring_t *in);

btstring_t	*bts_strdup(btstring_t *in);

#endif
