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

typedef struct {
	uint8_t		depth;
	uint8_t		*quad;
	uint8_t		*facet;
	uint8_t		index;
} cityTile_t;

#endif
