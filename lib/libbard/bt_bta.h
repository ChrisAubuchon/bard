#ifndef _BT_BTA_H
#define _BT_BTA_H

/*
 * bt_bta.h - Bard's Tale Animation file format
 */

#include <stdint.h>
#include <bt_string.h>

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define		BTA_NOBASE	0
#define		BTA_HASBASE	1

/********************************/
/*				*/
/* 				*/
/*				*/
/********************************/

typedef struct {
	uint16_t	x;
	uint16_t	y;

	uint16_t	width;
	uint16_t	height;

	uint16_t	delay;
	uint16_t	c_delay;

	btstring_t	*gfx;
} bta_cell_t;

typedef struct {
	void		*cellData;
} bta_loop_t;

typedef struct {
	bta_cell_t	*base;
	void		*loopData;
} bta_t;

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

bta_cell_t	*bta_cell_new(uint16_t x, uint16_t y, 
				uint16_t width, uint16_t height, 
				uint16_t delay, btstring_t *gfx);
void		 bta_cell_free(const void *vbtac);

bta_loop_t	*bta_loop_new(bta_t *bta, uint32_t lnum, uint32_t ncells);
void		bta_loop_free(const void *vbtal);

uint32_t	bta_getNumCells(bta_t *bta, uint32_t lnum);
uint32_t	bta_getNumLoops(bta_t *bta);

bta_t		*bta_new(uint32_t nloops);
void		bta_free(bta_t *bta);

void		bta_write_bta(btstring_t *fname, bta_t *bta);
bta_t		*bta_read_bta(btstring_t *fname);

bta_loop_t	*bta_loop_get(bta_t *bta, uint32_t lnum);
bta_cell_t 	*bta_cell_get(bta_t *bta, uint32_t loop, uint32_t cell);
void 		bta_cell_set(bta_t *bta, uint32_t lnum, uint32_t cnum, 
				bta_cell_t *img);
bta_cell_t	*bta_get_base(bta_t *bta);

#endif
