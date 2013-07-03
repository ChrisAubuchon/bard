#include <b1lib.h>
#include <gfx_bigpic.h>
#include <gfx.h>
#include <cnv_gfx.h>
#include <dehuf.h>
#include <fpio.h>

/*#define DEBUG */
#include <debug.h>

/****************************************/
/*					*/
/* Global variables			*/
/*					*/
/****************************************/
static animGfx_t *head = NULL;

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/

static void bigpic_flate(btstring_t * in, btstring_t ** big, btstring_t ** anim, uint16_t size);
void getAnimLoops(bta_t *bta, btstring_t *anim, uint8_t nloops);

/****************************************/
/*					*/
/* Animation structure prototypes	*/
/*					*/
/****************************************/
static animGfx_t *anim_newNode(void);
static void anim_initList(void);
static void anim_freeList(void);
static void anim_insert(animGfx_t * new, btstring_t * data);
static void anim_parse(btstring_t * data);

/****************************************/
/*					*/
/* Animation output prototypes		*/
/*					*/
/****************************************/

static uint8_t anim_countLoops(void);
static uint8_t anim_countCells(btstring_t * anim, uint16_t base_offset, uint16_t base_cycleCount);
static uint16_t anim_skipCell(btstring_t * anim, uint16_t offset);
static animbox_t *getAnimBox(btstring_t *data, uint16_t off);
static uint16_t copyCell(btstring_t * anim, uint16_t offset, btstring_t * big, animbox_t *box);

/****************************************/
/*					*/
/* Local functions			*/
/*					*/
/****************************************/

static void bigpic_flate(btstring_t * in, btstring_t ** big, btstring_t ** anim, uint16_t size)
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

void getAnimLoops(bta_t *bta, btstring_t *anim, uint8_t nloops)
{
	animGfx_t *cur;
	bta_loop_t *l;
	bta_cell_t *c;
	uint8_t i, j;
	uint16_t offset;
	int16_t cycleCount;
	uint8_t ncells;
	animbox_t *ab;

	cur = head;
	for (i = 0; i < nloops; i++) {
		while (cur->anims[i].base_offset == cur->anims[i].base_segment)
			cur = cur->next;

		ncells = anim_countCells(anim, cur->anims[i].cur_offset,
				cur->anims[i].cycleCount);
		l = bta_loop_new(bta, i, ncells);

		offset = cur->anims[i].cur_offset;
		cycleCount = cur->anims[i].cycleCount;

		for (j = 0; j < ncells; j++) {
			if (cycleCount == 0) {
				cycleCount = 1;
			}
			debug("delay = %d\n", cycleCount);
			ab = getAnimBox(anim, offset);

			c = bta_cell_new(ab->x, ab->y, ab->width, ab->height, 
				cycleCount, NULL);
			c->gfx = bts_new(ab->height * ab->width);

			offset = copyCell(anim, offset, c->gfx, ab);
			c = bta_cell_convert(c);

			bta_cell_set(bta, i, j, c);

			cycleCount = str_read16be(&anim->buf[offset]);
			offset += 2;

			free(ab);
		}
	}
}


/****************************************/
/*					*/
/* Animation structure functions	*/
/*					*/
/****************************************/

static animGfx_t *anim_newNode(void)
{
	animGfx_t *new;
	int i;

	new = (animGfx_t *) xzalloc(sizeof(animGfx_t));
	new->next = NULL;
	new->prev = NULL;
	for (i = 0; i < 5; i++) {
		new->anims[i].base_offset = 0;
		new->anims[i].base_segment = 0;
		new->anims[i].cur_offset = 0;
		new->anims[i].cur_segment = 0;
		new->anims[i].cycleCount = 0;
	}

	return new;
}

static void anim_initList(void)
{
	head = anim_newNode();
}

static void anim_freeList(void)
{
	animGfx_t *cur = head;
	animGfx_t *old;

	while (cur->next != NULL) {
		old = cur;
		cur = cur->next;
		free(old);
	}
}

static void anim_insert(animGfx_t * new, btstring_t * data)
{
	int counter = 0;
	animGfx_t *listp = head;

	while (listp->next != NULL)
		listp = listp->next;

	new->prev = listp;
	listp->next = new;

	while (new->anims[counter].base_offset != new->anims[counter].base_segment) {
		new->anims[counter].cur_offset = new->anims[counter].base_offset;
		new->anims[counter].cur_segment = new->anims[counter].base_segment;
		new->anims[counter].cycleCount = str_read16be(&data->buf[new->anims[counter].base_offset]);
		new->anims[counter].cur_offset += 2;
		counter++;
	}
}

static void anim_parse(btstring_t * data)
{
	animGfx_t *new;

	int counter;
	uint16_t off = 0;

	new = anim_newNode();

	for (counter = 0; counter < 5; counter++) {
/*		debug("off = %x\n", off);*/
/*		debug("data->buf[off] = %x\n", data->buf[off]);*/
/*		debug("str_read16be(&data->buf[off]) = %x\n", str_read16be(&data->buf[off]));*/
		if (str_read16be(&data->buf[off])) {
			new->anims[counter].base_offset = off + 2;
			new->anims[counter].base_segment = 1;
			off += ((str_read16be(&data->buf[off]) + 1) & 0xfffe) + 2;
		} else {
			new->anims[counter].base_offset = 0;
			new->anims[counter].base_segment = 0;
		}
	}

	anim_insert(new, data);
}


/****************************************/
/*					*/
/* Animation output functions		*/
/*					*/
/****************************************/

/*
 * anim_countLoops()
 * Count the number of loops an animation has
 */
static uint8_t anim_countLoops(void)
{
	uint8_t nloops = 0;
	animGfx_t *cur = head;

	while (cur != NULL) {
		if (cur->anims[nloops].base_offset == cur->anims[nloops].base_segment) {
			cur = cur->next;
			continue;
		}

		nloops++;
	}

	return nloops;
}

/*
 * anim_countCells()
 * Counter the number of cells in a loop
 */
static uint8_t anim_countCells(btstring_t * anim, uint16_t base_offset, uint16_t base_cycleCount)
{
	uint16_t offset = base_offset;
	int16_t cycleCount = base_cycleCount;
	uint8_t ncells = 0;

	while (cycleCount >= 0) {
		offset = anim_skipCell(anim, offset);
		cycleCount = str_read16be(&anim->buf[offset]);
		offset += 2;

		ncells++;
	}

	return ncells;
}

/*
 * anim_skipCell()
 * Skip the cell data for counting purposes
 */
static uint16_t anim_skipCell(btstring_t * anim, uint16_t offset)
{
	uint16_t o = offset;

	while (str_read16le(&anim->buf[o]) != 0xffff)
		o += 6;

	return o + 2;
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

		big->buf[index++] ^= anim->buf[o++];
		big->buf[index++] ^= anim->buf[o++];
		big->buf[index++] ^= anim->buf[o++];
		big->buf[index++] ^= anim->buf[o++];

		index += 4;
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
	bt_bigpicList_t *bpl;

	bta_t *img;
	bta_cell_t *c;
	uint8_t nloops;

	FILE *fp;

	fp = xfopen(mkBardOnePath("bigpic"), "rb");
	npics = fp_read32le(fp) / sizeof(uint32_t);

	bpl = bigpic_list_new(npics);

	for (i = 0; i < npics; i++) {
		debug("Picture %d:\n", i);
		fp_moveToIndex32(fp, i, 0);

		huf = dehuf_init(fp);
		data = dehuf(huf, 0x4074);

		bigpic_flate(data, &big, &anim, huf->maxSize);

		if (anim == NULL) {
			nloops = 0;
		} else {
			anim_initList();
			anim_parse(anim);

			nloops = anim_countLoops();
		}

		if (nloops) {
			img = bta_new(BTA_TYPE_XORLOOP, nloops);

			c = bta_cell_new(0, 0, 56, 88, 0, big);
			c = bta_cell_convert(c);
			img->base = c;

			getAnimLoops(img, anim, nloops);

			bts_free(anim);
			anim_freeList();

			bta_write_bta(mkImagePath("bigpic_%d.bta", i), img);

			bigpic_list_set(bpl, \
					getPicMacro(i),			\
					mkImagePathRel("bigpic_%d.bta", i),  \
					BTA_TYPE_XORLOOP);

			bta_free(img);
		} else {
			c = bta_cell_new(0, 0, 56, 88, 0, big);
			c = bta_cell_convert(c);
			bta_toPNG(c, mkImagePath("bigpic_%d.png", i));

			bigpic_list_set(bpl, \
					getPicMacro(i),			\
					mkImagePathRel("bigpic_%d.png", i),  \
					BTA_PNG);
			bta_cell_free(c);
		}

		anim = NULL;
		dehuf_free(huf);
	}

	bigpic_list_to_json(bpl, mkJsonPath("bigpic.json"));
	bigpic_list_free(bpl);

	fclose(fp);
}
