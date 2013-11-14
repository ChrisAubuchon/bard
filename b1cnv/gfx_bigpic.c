#include <b1lib.h>
#include <gfx_bigpic.h>
#include <gfx.h>
#include <cnv_gfx.h>
#include <cnv_bigpic.h>
#include <dehuf.h>
#include <fpio.h>

/*#define DEBUG */
#include <debug.h>

/****************************************/
/*					*/
/* Global variables			*/
/*					*/
/****************************************/

static uint8_t isTimeAware[] = {
/*  0*/	0, 0, 0, 0,
/*  4*/	0, 0, 0, 0,
/*  8*/	0, 0, 0, 0,
/* 12*/	0, 0, 0, 0,
/* 16*/	0, 0, 0, 0,
/* 20*/	0, 0, 0, 0,
/* 24*/	0, 0, 0, 0,
/* 28*/	0, 0, 0, 0,
/* 32*/	0, 0, 0, 1,
/* 36*/	1, 1, 1, 0,
/* 40*/	0, 0, 1, 1,
/* 44*/	0, 1, 0, 0,
/* 48*/	0, 0, 0, 0,
/* 52*/	1, 0, 0, 0,
/* 56*/	0, 0, 0, 0,
/* 60*/	0, 1, 0
};

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/

static void	bigpic_flate(btstring_t *in, btstring_t **big, 
				btstring_t **anim, uint16_t size);
static		bta_cell_t *cellToRGBA(bta_cell_t *in, btstring_t *big);
void		getAnimLoops(bta_t *bta, btstring_t *anim, anim_t *ba);

/****************************************/
/*					*/
/* Animation output prototypes		*/
/*					*/
/****************************************/

static anim_t	*anim_parse(btstring_t * data);
static uint8_t		anim_countCells(btstring_t *data, uint16_t base_offset);
static animbox_t *getAnimBox(btstring_t *data, uint16_t off);
static uint16_t	copyCell(btstring_t * anim, uint16_t offset, 
			btstring_t * big, animbox_t *box);

/****************************************/
/*					*/
/* Local functions			*/
/*					*/
/****************************************/

static void bigpic_flate(btstring_t *in, btstring_t **big, 
			btstring_t **anim, uint16_t size)
{
	uint8_t *src;
	uint8_t *dest;
	uint8_t count;
	uint32_t i;

	src = in->buf;

	*big = bts_new(0x1340);
	dest = (*big)->buf;

	while ((count = *src++) != 0) {
		if (count & 0x80) {
			count &= 0x7f;
			for (; count != 0; count--)
				*dest++ = *src++;
		} else {
			for (; count != 0; count--)
				*dest++ = *src;
			src++;
		}
	}

	if ((src - in->buf) < size) {
		*anim = bts_new(in->size - (src - in->buf));
		dest = (*anim)->buf;
		for (i = 0; i < (*anim)->size; i++)
			*dest++ = *src++;
	} else {
		*anim = NULL;
	}
}

/*
 * getAnimLoops()
 */
void getAnimLoops(bta_t *bta, btstring_t *anim, anim_t *ba)
{
	bta_loop_t	*l;
	bta_cell_t	*c;
	uint8_t		i, j;
	uint16_t	offset;
	int16_t		cycleCount;
	uint8_t		ncells;
	animbox_t	*ab;
	btstring_t	*buf;


	for (i = 0; i < ba->nloops; i++) {
		ncells = anim_countCells(anim, ba->base[i].offset);
		l = bta_loop_new(bta, i, ncells);

		offset = ba->base[i].offset;
		cycleCount = ba->base[i].cycles;
		offset += 2;

		buf = bts_copy(bta->base->gfx);

		for (j = 0; j < ncells; j++) {
			if (cycleCount == 0) {
				cycleCount = 1;
			}
			ab = getAnimBox(anim, offset);

			c = bta_cell_new(ab->x, ab->y, ab->width, ab->height, 
				cycleCount, NULL);
			c->gfx = bts_new(ab->height * ab->width);

			offset = copyCell(anim, offset, c->gfx, ab);
			dump_btstring(bts_strcpy("cell0-pre8.dat"), c->gfx, 0);
			c = bta_cell_4bitTo8bit(c);
			c = bta_cell_scale(c);
			c = cellToRGBA(c, buf);

			bta_toPNG(c, mkImagePath("loop_%d_cell_%d.png", i, j));

			bta_cell_set(bta, i, j, c);

			cycleCount = str_read16be(&anim->buf[offset]);
			offset += 2;

			free(ab);
		}

		bts_free(buf);
	}
}

/*
 * cellToRGBA()
 *
 * Convert a palettized and XOR'd cell to a transparent/RGBA cell
 *
 * XXX: This might remove the need for the BTA_TYPE_XORLOOP...
 */
static bta_cell_t *cellToRGBA(bta_cell_t *in, btstring_t *big)
{
	bta_cell_t	*rval;
	uint32_t	i, j;
	uint8_t		*inp;
	uint8_t		*rvalp;
	uint32_t	index;

	rval = bta_cell_new(in->x, in->y, in->width, in->height, in->delay, NULL);
	rval->gfx = bts_new(in->gfx->size * 4);

	inp = in->gfx->buf;
	rvalp = rval->gfx->buf;

	debug("*******************************\n");
	debug("in->width:  %3d\n", in->width);
	debug("in->height: %3d\n", in->height);
	debug("in->x:      %3d\n", in->x);
	debug("in->y:      %3d\n", in->y);
	debug("in-delay:   %3d\n", in->delay);

	for (i = 0; i < in->height; i++) {
		for (j = 0; j < in->width; j++) {
			if (*inp == 0) {
				*rvalp++ = 0;
				*rvalp++ = 0;
				*rvalp++ = 0;
				*rvalp++ = 0;
				inp++;
			} else {
				index = ((224 * in->y) + (in->x)) + ((i * 224) + j);
				big->buf[index] ^= *inp++;
				*rvalp++ = egapal[big->buf[index]].red;
				*rvalp++ = egapal[big->buf[index]].green;
				*rvalp++ = egapal[big->buf[index]].blue;
				*rvalp++ = egapal[big->buf[index]].alpha;
			}
		}
	}

	debug("*******************************\n");

	bta_cell_free(in);
	
	return rval;
}

/****************************************/
/*					*/
/* Animation structure functions	*/
/*					*/
/****************************************/

static anim_t *anim_parse(btstring_t *data)
{
	uint16_t	offset		= 0;
	uint16_t	deltaOffset;
	uint16_t	loopOffset;
	uint16_t	loopCount;
	uint16_t	loopCells;
	anim_t		*rval;

	rval = (anim_t *)xzalloc(sizeof(anim_t));
	rval->nloops = 0;

	while (1) {
		loopOffset = offset + 2;
		loopCount = str_read16be(&data->buf[loopOffset]);

		deltaOffset = str_read16be(&data->buf[offset]);
		if (!deltaOffset)
			break;
		offset += deltaOffset + 2;

		debug("=====================================\n");
		debug("Loop %2d:\n", rval->nloops);
		debug("Base Offset: 0x%04x\n", loopOffset);
		debug("Cycle count: %3d\n", loopCount);

		loopCells = anim_countCells(data, loopOffset);
		debug("Loop cells: %3d\n", loopCells);
		if (!loopCells) {
			continue;
		}
		rval->base[rval->nloops].offset = loopOffset;
		rval->base[rval->nloops].cycles = loopCount;
		rval->nloops++;
		if (rval->nloops > 5)
			break;
	}

	return rval;
}


/****************************************/
/*					*/
/* Animation output functions		*/
/*					*/
/****************************************/

/*
 * anim_countCells()
 * Counter the number of cells in a loop
 */
static uint8_t anim_countCells(btstring_t *data, uint16_t base_offset)
{

	uint16_t	cycles;
	uint16_t	offset = base_offset;
	uint8_t		ncells = 0;

	while ((cycles = str_read16be(&data->buf[offset])) != 0xffff) {
/*		debug("offset: 0x%04x\n", offset);*/
		offset += 2;
		while (str_read16le(&data->buf[offset]) != 0xffff)
			offset += 6;
		offset += 2;
		ncells++;
	} 

	return ncells;
}

/*
 * getAnimBox()
 * Determine the (x,y) coordinates of the upper left hand corner
 * of the cell and also the width and height of the cell
 */
static animbox_t *getAnimBox(btstring_t *data, uint16_t off)
{
	uint8_t x_lo, y_lo;
	uint8_t x_hi, y_hi;
	uint16_t o = off;
	uint8_t x, y;
	animbox_t *box;

	box = (animbox_t *)xzalloc(sizeof(animbox_t));

	x_lo = y_lo = 0xff;
	x_hi = y_hi = 0;

	while (str_read16le(&data->buf[o]) != 0xffff) {
/*		debug("o: 0x%04x\n", o);*/
		y = data->buf[o] - 15;
		x = data->buf[o + 1] - 8;
		o += 2;

		if (x < x_lo)
			x_lo = x;
		if (y < y_lo)
			y_lo = y;
		if ((x + 4) > x_hi)
			x_hi = x + 4;
		if (y > y_hi)
			y_hi = y;

		o += 4;
	}

	box->x = x_lo;
	box->y = y_lo;

	box->width = x_hi - x_lo;
	box->height = (y_hi - y_lo) + 1;

	return box;
}

static uint16_t copyCell(btstring_t *anim, uint16_t offset, btstring_t *big, animbox_t *box)
{
	uint16_t index = 0;
	uint8_t x, y;
	uint16_t o = offset;

	while (str_read16le(&anim->buf[o]) != 0xffff) {
		y = anim->buf[o] - 15;
		x = anim->buf[o + 1] - 8;
		o += 2;

		index = ((y - box->y) * (box->width)) + (x - box->x);

		big->buf[index++] = anim->buf[o++];
		big->buf[index++] = anim->buf[o++];
		big->buf[index++] = anim->buf[o++];
		big->buf[index++] = anim->buf[o++];
	}

	return o + 2;
}

void outputBigpic(uint8_t indent)
{
	int i;
	huffile_t *huf;
	btstring_t *data;
	btstring_t *big;
	btstring_t *anim;
	uint16_t npics;
	cnvList_t	*bpl;
	bt_bigpic_t	*bp;

	bta_t *img;
	bta_cell_t *c;
	uint8_t nloops;

	FILE		*fp;
	anim_t		*ba;

	fp = xfopen(mkBardOnePath("bigpic"), "rb");
	npics = fp_read32le(fp) / sizeof(uint32_t);

	bpl = bigpicList_new();

	for (i = 0; i < npics; i++) {
/*	i = 0; {*/
		debug("Picture %d:\n", i);
		fp_moveToIndex32(fp, i, 0);

		huf = dehuf_init(fp);
		data = dehuf(huf, 0x4074);

		bigpic_flate(data, &big, &anim, huf->maxSize);

		if (anim == NULL) {
			nloops = 0;
		} else {
			ba = anim_parse(anim);
			nloops = ba->nloops;
		}

		bp = bigpic_new();
		bp->name = getPicMacro(i);
		bp->isTimeAware = isTimeAware[i];

		if (nloops) {
			img = bta_new(nloops);

			c = bta_cell_new(0, 0, 56, 88, 0, big);
			c = bta_cell_4bitTo8bit(c);
			c = bta_cell_scale(c);
			img->base = c;
 
			getAnimLoops(img, anim, ba);

			img->base = bta_cell_toRGBA(img->base, egapal);
			
			bts_free(anim);

			bta_write_bta(mkImagePath("bigpic_%d.bta", i), img);

			bp->type = IMAGE_BTA;
			bp->path = mkImagePathRel("bigpic_%d.bta", i);

			bta_free(img);
			free(ba);
		} else {
			c = bta_cell_new(0, 0, 56, 88, 0, big);
			if (isTimeAware[i]) {
				c = bta_cell_4bitTo8bit(c);
				c = bta_cell_scale(c);
				c = bta_cell_toRGBA(c, cityPalette);
			} else {
				c = bta_cell_convert(c);
			}
			bta_toPNG(c, mkImagePath("bigpic_%d.png", i));

			bp->type = IMAGE_PNG;
			bp->path = mkImagePathRel("bigpic_%d.png", i);
			bta_cell_free(c);
		}

		cnvList_add(bpl, bp);
		anim = NULL;
		dehuf_free(huf);
	}

	bigpicList_toJson(bpl, mkJsonPath("bigpic.json"));
	cnvList_free(bpl);

	fclose(fp);
}
