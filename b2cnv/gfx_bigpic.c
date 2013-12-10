#include <b2lib.h>
#include <gfx_bigpic.h>
#include <gfx.h>
#include <bt_gfx.h>
#include <cnv_gfx.h>
#include <cnv_bigpic.h>
#include <dehuf.h>
#include <fpio.h>

/*#define DEBUG */
#include <debug.h>

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/

static void		getAnimLoops(bta_t *bta, btstring_t *anim, 
					b2anim_t *b2a);
static void		bigpic_setBG(btstring_t *in);
static void		extractPixel(uint16_t data, pixel_t *p);
static bta_cell_t	*cellToRGBA(bta_cell_t *in, btstring_t *big);

/****************************************/
/*					*/
/* Animation structure prototypes	*/
/*					*/
/****************************************/
static b2anim_t		*anim_parse(btstring_t * data);

/****************************************/
/*					*/
/* Animation output prototypes		*/
/*					*/
/****************************************/

static uint8_t		anim_countCells(btstring_t * anim, 
					uint16_t base_offset);
static animbox_t	*getAnimBox(btstring_t *data, uint16_t off);
static void		copyCell(btstring_t *anim, uint16_t offset, 
					btstring_t *big, animbox_t *ab);

/****************************************/
/*					*/
/* Local functions			*/
/*					*/
/****************************************/

static void extractPixel(uint16_t data, pixel_t *p)
{
	p->x 	= (data & 0x0f) << 2;
	p->y	= (data & 0x07f0) >> 4;
	p->count= (data & 0x7000) >> 12;
}

static void bigpic_setBG(btstring_t * in)
{
	uint32_t i;

	for (i = 0x38; i < 0x1340; i++)
		in->buf[i] ^= in->buf[i - 0x38];
}

static void dumpBig(btstring_t *fname, btstring_t *big)
{
	bta_cell_t	*c;
	btstring_t	*tmp;

	tmp = bts_copy(big);

	c = bta_cell_new(0, 0, 224, 176, 0, tmp);
	c = bta_cell_toRGBA(c, egapal);
	bta_toPNG(c, fname);
	bta_cell_free(c);
}

static void getAnimLoops(bta_t *bta, btstring_t *anim, b2anim_t *b2a)
{
	bta_loop_t	*l;
	bta_cell_t	*c;
	uint8_t		i, j;
	uint16_t	offset, off;
	int16_t		cycleCount;
	uint8_t		ncells;
	animbox_t	*ab;
	btstring_t	*buf;

	for (i = 0; i < b2a->nloops; i++) {
		debug("*************************************\n");
		ncells = anim_countCells(anim, b2a->base[i].offset);
		l = bta_loop_new(bta, i, ncells);

		offset = b2a->base[i].offset;
		cycleCount = b2a->base[i].cycles;

		buf = bts_copy(bta->base->gfx);

		for (j = 0; j < ncells; j++) {	
			debug("Cell: %d\n", j);
			if (cycleCount == 0) {
				cycleCount = 1;
			}

			off = b2a->base[i].offset + str_read16le(&anim->buf[offset + 2]);
			ab = getAnimBox(anim, off);

			c = bta_cell_new(ab->x, ab->y, 
					ab->width, ab->height, 
					cycleCount, NULL
					);
			c->gfx = bts_new(ab->height * ab->width);

			copyCell(anim, off, c->gfx, ab);
			c = bta_cell_4bitTo8bit(c);
			c = bta_cell_scale(c);
			c = cellToRGBA(c, buf);

			bta_cell_set(bta, i, j, c);

			offset += 4;
			cycleCount = str_read16le(&anim->buf[offset]);

			free(ab);
		}

		bts_free(buf);
	}
}

/*
 * celltoRGBA()
 */
static bta_cell_t *cellToRGBA(bta_cell_t *in, btstring_t *big)
{
	bta_cell_t	*rval;
	uint32_t	i, j;
	uint8_t		*inp;
	uint8_t		*rvalp;
	uint32_t	index;

	rval = bta_cell_new(
		in->x, in->y,
		in->width, in->height,
		in->delay, NULL);
	rval->gfx = bts_new(in->gfx->size * 4);

	inp = in->gfx->buf;
	rvalp = rval->gfx->buf;

	for (i = 0; i < in->height; i++) {
		for (j = 0; j < in->width; j++) {
			if (*inp == 0) {
				*rvalp++ = 0;
				*rvalp++ = 0;
				*rvalp++ = 0;
				*rvalp++ = 0;
				*inp++;
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

	bta_cell_free(in);

	return rval;
}

/****************************************/
/*					*/
/* Animation structure functions	*/
/*					*/
/****************************************/

/*
 * anim_parse()
 */
static b2anim_t *anim_parse(btstring_t *data)
{
	uint16_t	offset		= 0;
	uint16_t	deltaOffset;
	uint16_t	loopOffset;
	uint16_t	loopCount;
	uint16_t	loopCells;
	b2anim_t	*rval;

	rval = (b2anim_t *)xzalloc(sizeof(b2anim_t));
	rval->nloops = 0;

	while (1) {
		loopOffset = offset + 2;
		loopCount = str_read16le(&data->buf[loopOffset]);

		deltaOffset = str_read16le(&data->buf[offset]);
		if (!deltaOffset) 
			break;
		offset += deltaOffset + 2;

		debug("===================================\n");
		debug("Loop %2d:\n", rval->nloops);
		debug("Base Offset: 0x%04x\n", loopOffset);
		debug("Cycle count: %3d\n", loopCount);

		loopCells = anim_countCells(data, loopOffset);
		debug("Loop cells: %3d\n", loopCells);

		if (!loopCells) {
			debug("Skipping loop %d\n", rval->nloops);
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
static uint8_t anim_countCells(btstring_t *anim, uint16_t base_offset)
{
	b2animCell_t	cell;
	uint16_t	offset	= base_offset;
	uint8_t		ncells	= 0;

#if 0
	debug("base_offset: 0x%04x\n", base_offset);
#endif

	do {
		cell.cycles = str_read16le(&anim->buf[offset]);
		cell.offset = str_read16le(&anim->buf[offset + 2]) + offset;
		offset += 4;

		if (str_read16le(&anim->buf[cell.offset]) == 0xffff)
			break;

#if 0
		debug("cell %d:\n", ncells);
		debug("  cycleCount: %4d\n", cell.cycles);
		debug("  cellOffset: 0x%04x\n", cell.offset);
#endif

		ncells++;
	} while (cell.cycles != 0xffff);
		
	return ncells;
}

static animbox_t *getAnimBox(btstring_t *data, uint16_t off)
{
	uint8_t		x_lo, y_lo;
	uint8_t		x_hi, y_hi;
	uint8_t		x, y;
	uint16_t	o = off;
	uint16_t	pixel;
	pixel_t		p;
	animbox_t *box;

	box = (animbox_t *)xmalloc(sizeof(animbox_t));

	x_lo = y_lo = 0xff;
	x_hi = y_hi = 0;

	pixel = str_read16le(&data->buf[o]);
	o += 2;

	while (pixel != 0xffff) {
		extractPixel(pixel, &p);
		y = p.y;
		x = p.x;

		if (x < x_lo)
			x_lo = x;
		if (y < y_lo)
			y_lo = y;

		x += (4 * (p.count + 1));
		if ((x) > x_hi)
			x_hi = x;
		if (y > y_hi)
			y_hi = y;

		o += 4 * (p.count + 1);
		pixel = str_read16le(&data->buf[o]);
		o += 2;
	}

	box->x = x_lo;
	box->y = y_lo;

	box->width = x_hi - x_lo;
	box->height = (y_hi - y_lo) + 1;

	return box;
}

static void copyCell(btstring_t *anim, uint16_t offset, btstring_t *big, animbox_t *cell)
{
	uint16_t	index = 0;
	uint16_t	o = offset;
	uint16_t	pixel;
	uint16_t	counter;
	uint32_t	i;
	pixel_t		p;

	pixel = str_read16le(&anim->buf[o]);
	o += 2;

	while (pixel != 0xffff) {
		extractPixel(pixel, &p);
		counter = p.count + 1;
		index = ((p.y - cell->y) * (cell->width)) + (p.x - cell->x);

		for (i = 0; i < counter; i++) {
			big->buf[index++] ^= anim->buf[o++];
			big->buf[index++] ^= anim->buf[o++];
			big->buf[index++] ^= anim->buf[o++];
			big->buf[index++] ^= anim->buf[o++];
		}

		pixel = str_read16le(&anim->buf[o]);
		o += sizeof(int16_t);
	}
}

void outputBigpic(void)
{
	int		i;
	huffile_t	*huf;
	btstring_t	*data;
	btstring_t	*big;
	btstring_t	*anim = NULL;
	uint16_t	npics;
	cnvList_t	*bpl;
	bt_bigpic_t	*bp;

	bta_t		*img;
	bta_cell_t	*c;
	uint8_t		nloops;

	FILE 		*fp;
	b2anim_t	*b2a;

	fp = xfopen(mkBardTwoPath("BIGPIC"), "rb");
	npics = fp_read32le(fp) / sizeof(uint32_t);

	bpl = bigpicList_new();

	for (i = 0; i < npics; i++) {
		debug("Bigpic: %d\n", i);
		fp_moveToIndex32(fp, i, 0);

		huf = dehuf_init(fp);
		big = dehuf(huf, 0x1340);

		if (huf->maxSize > 0x1340)
			anim = dehuf(huf, 0x32c3);

		if (anim == NULL) {
			nloops = 0;
		} else {
			b2a = anim_parse(anim);
			nloops = b2a->nloops;
		}

		bp = bigpic_new();
		bp->name = getPicMacro(i);
#if 0
		bp->isTimeAware = isTimeAware[i];
#endif

		if (nloops) {
			img = bta_new(b2a->nloops);

			bigpic_setBG(big);
			c = bta_cell_new(0, 0, 56, 88, 0, big);
			c = bta_cell_4bitTo8bit(c);
			c = bta_cell_scale(c);
			img->base = c;

			getAnimLoops(img, anim, b2a);

			img->base = bta_cell_toRGBA(img->base, egapal);

			bts_free(anim);

			bta_write_bta(mkImagePath("bigpic_%d.bta", i), img);

			bp->type = IMAGE_BTA;
			bp->path = mkImagePathRel("bigpic_%d.bta", i);

			bta_free(img);
			free(b2a);
		} else {
			bigpic_setBG(big);
			c = bta_cell_new(0, 0, 56, 88, 0, big);
			c = bta_cell_convert(c);
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
