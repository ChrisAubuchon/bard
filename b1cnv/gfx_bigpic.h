#ifndef _GFX_ANIM_H
#define _GFX_ANIM_H

#define COMPRESS_MIN 128

#include <b1lib.h>

typedef struct {
  uint16_t base_offset;
  uint16_t base_segment;
  uint16_t cur_offset;
  uint16_t cur_segment;
  int16_t cycleCount;
} _anim_t;

typedef struct _animGfx_t {
  _anim_t anims[5];
  struct _animGfx_t *next;
  struct _animGfx_t *prev;
} animGfx_t;

typedef struct {
  uint8_t x;
  uint8_t y;
  uint8_t width;
  uint8_t height;
} animbox_t;

typedef struct {
	uint16_t	cycles;
	uint16_t	offset;
} animCell_t;

typedef struct {
	uint8_t		nloops;
	animCell_t	base[5];
} anim_t;

#endif
