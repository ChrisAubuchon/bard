#include <b2lib.h>
#include <gfx_bigpic.h>
#include <gfx.h>
#include <bt_gfx.h>
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

static void getAnimLoops(bta_t *bta, btstring_t *anim, uint8_t nloops);
static void bigpic_setBG(btstring_t * in);
static void extractPixel(uint16_t data, pixel_t *p);

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
static animbox_t *getAnimBox(btstring_t *data, uint16_t off);
static void copyCell(btstring_t *anim, uint16_t offset, btstring_t *big, animbox_t *ab);

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

static void getAnimLoops(bta_t *bta, btstring_t *anim, uint8_t nloops)
{
	animGfx_t	*cur;
	bta_loop_t	*l;
	bta_cell_t	*c;
	uint8_t		i, j;
	uint16_t	offset, off;
	int16_t		cycleCount;
	uint8_t		ncells;
	animbox_t	*ab;

	cur = head;
	for (i = 0; i < nloops; i++) {
		debug("*************************************\n");
		while (cur->anims[i].base_offset == cur->anims[i].base_segment)
			cur = cur->next;

		ncells = anim_countCells(anim, cur->anims[i].cur_offset, 
					cur->anims[i].cycleCount);
		l = bta_loop_new(bta, i, ncells);

		offset = cur->anims[i].cur_offset;
		cycleCount = cur->anims[i].cycleCount;

		for (j = 0; j < ncells; j++) {	
			debug("Cell: %d\n", j);
			if (cycleCount == 0) {
				cycleCount = 1;
			}

			off = cur->anims[i].base_offset + str_read16le(&anim->buf[offset + 2]);
			ab = getAnimBox(anim, off);

			c = bta_cell_new(ab->x, ab->y, 
					ab->width, ab->height, 
					cycleCount, NULL
					);
			c->gfx = bts_new(ab->height * ab->width);

			copyCell(anim, off, c->gfx, ab);
			c = bta_cell_convert(c);
			
			debug("c->x = %d, c->y = %d, c->w = %d, c->h = %d, c->delay = %d\n", c->x, c->y, c->width, c->height, cycleCount);

			bta_cell_set(bta, i, j, c);

			offset += 4;
			cycleCount = str_read16le(&anim->buf[offset]);

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

	new = (animGfx_t *) xmalloc(sizeof(animGfx_t));
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

	while (cur != NULL) {
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
		new->anims[counter].cycleCount = str_read16le(&data->buf[new->anims[counter].base_offset]);
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
		debug("off = %x\n", off);
		if (str_read16le(&data->buf[off])) {
			new->anims[counter].base_offset = off + 2;
			new->anims[counter].base_segment = 1;
			off += (str_read16le(&data->buf[off]) & 0xfffe) + 2;
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
		offset += 4;
		cycleCount = str_read16be(&anim->buf[offset]);

		ncells++;
	}

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
	bt_bigpicList_t	*bpl;

	bta_t		*img;
	bta_cell_t	*c;
	uint8_t		nloops;

	FILE 		*fp;

	fp = xfopen(mkBardTwoPath("BIGPIC"), "rb");
	npics = fp_read32le(fp) / sizeof(uint32_t);

	bpl = bigpic_list_new(npics);

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
			anim_initList();
			anim_parse(anim);

			nloops = anim_countLoops();
		}

		if (nloops) {
			img = bta_new(BTA_TYPE_XORLOOP, nloops);

			bigpic_setBG(big);
			c = bta_cell_new(0, 0, 56, 88, 0, big);
			c = bta_cell_convert(c);
			img->base = c;

			getAnimLoops(img, anim, nloops);

			bts_free(anim);
			anim_freeList();

			bta_write_bta(mkImagePath("bigpic_%d.bta", i), img);

			bigpic_list_set(bpl,
				getPicMacro(i),
				mkImagePathRel("bigpic_%d.bta", i),
				BTA_TYPE_XORLOOP
				);

			bta_free(img);
		} else {
			bigpic_setBG(big);
			c = bta_cell_new(0, 0, 56, 88, 0, big);
			c = bta_cell_convert(c);
			bta_toPNG(c, mkImagePath("bigpic_%d.png", i));

			bigpic_list_set(bpl,
				getPicMacro(i),
				mkImagePathRel("bigpic_%d.png", i),
				BTA_PNG
				);

			bta_cell_free(c);
		}

		anim = NULL;
		dehuf_free(huf);
	}

	bigpic_list_to_json(bpl, mkJsonPath("bigpic.json"));
	bigpic_list_free(bpl);

	fclose(fp);
}
