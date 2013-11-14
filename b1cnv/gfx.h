#ifndef _B1_GFX_H
#define _B1_GFX_H

#include <cnv_gfx.h>

typedef struct {
  int16_t offset;
  int16_t width;
  int16_t height;
} btwoGfx_t;

typedef struct {
  uint16_t left;
  uint16_t bottom;
  uint16_t right;
  uint16_t top;
} oldGfxSZ_t;

typedef struct {
  uint16_t x_lo;
  uint16_t y_lo;
  uint16_t x_hi;
  uint16_t y_hi;
} oldGfxXY_t;

typedef struct {
  oldGfxSZ_t blist[4];
} bldgGfx_t;

typedef struct {
  uint16_t size;
  uint8_t *name;
  uint8_t width;
  uint8_t height;
	uint8_t delay;
} iconGfx_t;

typedef struct {
	uint16_t left;
	uint16_t bottom;
	uint16_t right;
	uint16_t top;
	uint16_t x;
	uint16_t y;
	uint8_t rflag;
	uint16_t offset;
	uint16_t width;
} view_t;

typedef struct {
	uint8_t *name;
	uint8_t *extra;
	uint8_t depth;
	uint8_t rightFlag;
	uint8_t faceMap;
} dunfacet_t;

bta_cell_t *getImage(view_t *view, btstring_t *data);

extern bta_color_t cityPalette[];

#endif
