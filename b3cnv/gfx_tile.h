#ifndef __GFX_TILE_H
#define __GFX_TILE_H

typedef struct {
	uint16_t	base;
	uint8_t		width;
	uint8_t		height;
	uint8_t		scale;
	uint8_t		srcSkip;
	uint8_t		rightFlag;
	bta_cell_t	*cell;
} b3tile_t;

#endif
