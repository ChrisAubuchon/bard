#ifndef _ENDIAN_H
#define _ENDIAN_H

#include <stdint.h>
#include <sys/param.h>

#if BYTE_ORDER == BIG_ENDIAN

#define cnv16be(x) (x)
#define cnv16le(x) ((((x)&0x00ff) << 8) | (((x)&0xff00) >> 8))
#define cnv32be(x) (x)
#define cnv32le(x) (((x) << 24) | (((x) & 0xff00) << 8) | (((x) & 0xff0000) >> 8) | (x) >> 24)

#else

#define cnv16be(x) (((x) << 8) | ((x) >> 8))
#define cnv16le(x) (x)
#define cnv32be(x) (((x) << 24) | (((x) & 0xff00) << 8) | (((x) & 0xff0000) >> 8) | (x) >> 24)
#define cnv32le(x) (x)

#endif

uint16_t str_read16be(uint8_t *buf);
uint16_t str_read16le(uint8_t *buf);
uint32_t str_read32be(uint8_t *buf);
uint32_t str_read32le(uint8_t *buf);
uint16_t file_read16be(int fd);
uint16_t file_read16le(int fd);
uint32_t file_read32be(int fd);
uint32_t file_read32le(int fd);

void file_write16le(int fd, uint16_t val);
void file_write32le(int fd, uint32_t val);

#endif
