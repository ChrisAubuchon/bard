#include <b2lib.h>
#include <gfx.h>
#include <cnv_gfx.h>

/*#define DEBUG*/
#include <debug.h>

#define DUN_FRONT	0x01
#define DUN_SIDE	0x02
#define DUN_PORTAL	0x04


/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/

static void dun_gfxInit();
static void dun_outputGfx(uint8_t indent, uint8_t dpic, uint8_t quad, uint8_t face);
static void output_gfxXML(uint8_t indent, uint8_t * tag, btstring_t * gfx);

static void outputBackground(uint8_t tileno);
static void getDunGfx(uint8_t dunno, uint8_t quad);
static void outputDunFacet(uint8_t dunno, uint8_t facet, uint8_t quad);
static btstring_t *_getDunGfx(uint16_t offset, int16_t _src_skip, int16_t width, int16_t height);

/****************************************/
/*					*/
/* Static variables			*/
/*					*/
/****************************************/

static uint8_t quadmap[] = {
	QUAD_5WL, QUAD_5FL, QUAD_5L,
	QUAD_4WL, QUAD_4FL, QUAD_4L,
	QUAD_3FL, QUAD_3L,
	QUAD_2L,
	QUAD_1L,
	QUAD_5WR, QUAD_5FR, QUAD_5R,
	QUAD_4WR, QUAD_4FR, QUAD_4R,
	QUAD_3FR, QUAD_3R,
	QUAD_2R,
	QUAD_1R,
	QUAD_4M, QUAD_3M, QUAD_2M,
	QUAD_1M
};

static uint8_t qfacets[] = {
/*5WL*/ DUN_SIDE,
/*5FL*/ DUN_SIDE,
/*5L */ DUN_SIDE,
/*4WL*/ DUN_SIDE | DUN_FRONT,
/*4FL*/ DUN_SIDE | DUN_FRONT,
/*4L */ DUN_SIDE | DUN_FRONT | DUN_PORTAL,
/*3FL*/ DUN_SIDE | DUN_FRONT,
/*3L */ DUN_SIDE | DUN_FRONT | DUN_PORTAL,
/*2L */ DUN_SIDE | DUN_FRONT | DUN_PORTAL,
/*1L */ DUN_SIDE | DUN_FRONT,
/*5WR*/ DUN_SIDE,
/*5FR*/ DUN_SIDE,
/*5R */ DUN_SIDE,
/*4WR*/ DUN_SIDE | DUN_FRONT,
/*4FR*/ DUN_SIDE | DUN_FRONT,
/*4R */ DUN_SIDE | DUN_FRONT | DUN_PORTAL,
/*3FR*/ DUN_SIDE | DUN_FRONT,
/*3R */ DUN_SIDE | DUN_FRONT | DUN_PORTAL,
/*2R */ DUN_SIDE | DUN_FRONT | DUN_PORTAL,
/*1R */ DUN_SIDE | DUN_FRONT,
/*4M */ DUN_FRONT | DUN_PORTAL,
/*3M */ DUN_FRONT | DUN_PORTAL,
/*2M */ DUN_FRONT | DUN_PORTAL,
/*1M */ DUN_FRONT
};

static uint8_t nfacets[] = {
/*5WL*/ 1,
/*5FL*/ 1,
/*5L */ 1,
/*4WL*/ 2,
/*4FL*/ 2,
/*4L */ 3,
/*3FL*/ 2,
/*3L */ 3,
/*2L */ 3,
/*1L */ 2,
/*5WR*/ 1,
/*5FR*/ 1,
/*5R */ 1,
/*4WR*/ 2,
/*4FR*/ 2,
/*4R */ 3,
/*3FR*/ 2,
/*3R */ 3,
/*2R */ 3,
/*1R */ 2,
/*4M */ 2,
/*3M */ 2,
/*2M */ 2,
/*1M */ 1
};

static uint8_t quadrightflag[] = { 0, 0, 0,
	0, 0, 0,
	0, 0,
	0,
	0,
	1, 1, 1,
	1, 1, 1,
	1, 1,
	1,
	1,
	0, 0, 0, 0
};

static btwoGfx_t dun_frontGfx[11];
static btwoGfx_t dun_portalGfx[6];
static btwoGfx_t dun_sideGfx[10];

static btstring_t *dundata;

static int8_t dunFrontQuadMap[] = { -1, -1, -1,
	10, 9, 7,
	8, 6,
	5,
	4,
	-1, -1, -1,
	10, 9, 7,
	8, 6,
	5,
	4,
	3, 2, 1, 0
};

static int8_t dunSideQuadMap[] = { 9, 6, 4,
	8, 5, 3,
	7, 2,
	1,
	0,
	9, 6, 4,
	8, 5, 3,
	7, 2,
	1,
	0,
	-1, -1, -1, -1
};

static int8_t dunPortQuadMap[] = {
	-1, -1, -1,
	-1, -1, 5,
	-1, 4,
	3,
	-1,
	-1, -1, -1,
	-1, -1, 5,
	-1, 4,
	3,
	-1,
	2, 1, 0, -1
};

static oldGfxXY_t dunFrontXY[] = {
	{9, 0, 103, 85}, {29, 13, 83, 66}, {40, 24, 72, 56}, {48, 33, 64, 49},
	{0, 0, 8, 85}, {0, 13, 28, 66}, {7, 24, 39, 56}, {31, 33, 47, 49},
	{0, 24, 6, 56}, {14, 33, 30, 49}, {0, 33, 13, 49}
};

static oldGfxSZ_t dunFrontSZ[] = {
	{1, 0, 95, 85}, {5, 0, 59, 53}, {0, 0, 32, 32}, {0, 0, 16, 16},
	{87, 0, 95, 85}, {31, 0, 59, 53}, {0, 0, 32, 32}, {0, 0, 16, 16},
	{26, 0, 32, 32}, {0, 0, 16, 16}, {3, 0, 16, 16}
};

static uint16_t dunFrontDoorOffset[] = {
	96, 56, 40, 24, 96, 56, 40, 24, 40, 24, 24
};

static oldGfxXY_t dunPortXY[] = {
	{48, 72, 63, 78}, {48, 61, 63, 63}, {48, 52, 63, 53}, {0, 72, 7, 77}, {8, 60, 23, 62},
	{24, 52, 39, 53}
};

static oldGfxSZ_t dunPortSZ[] = {
	{0, 0, 15, 6}, {0, 0, 15, 2}, {0, 0, 15, 1}, {0, 0, 7, 5}, {0, 0, 15, 2}, {0, 0, 15, 1}
};

static uint16_t dunPortFloorOffset[] = {
	16, 16, 16, 8, 16, 16
};

static oldGfxXY_t dunSideXY[] = {
	{0, 0, 8, 87}, {9, 0, 28, 84}, {29, 14, 39, 65}, {40, 25, 47, 55}, {48, 34, 51, 48},
	{8, 24, 29, 55}, {30, 32, 42, 49}, {0, 21, 7, 60}, {0, 24, 13, 55}, {14, 32, 26, 49}
};

static oldGfxSZ_t dunSideSZ[] = {
	{0, 0, 8, 87}, {1, 0, 20, 84}, {5, 0, 15, 51}, {0, 0, 7, 30}, {0, 0, 3, 14},
	{0, 0, 21, 31}, {6, 0, 18, 17}, {0, 0, 7, 39}, {8, 0, 21, 31}, {6, 0, 18, 17}
};

static uint16_t dunSideDoorOffset[] = {
	16, 24, 16, 8, 8, 24, 16, 8, 24, 16
};

/****************************************/
/*					*/
/* Local functions			*/
/*					*/
/****************************************/

static void outputDunFacet(uint8_t dunno, uint8_t facet, uint8_t quad)
{
	bta_cell_t *img;

	uint16_t doorOffset = 0;
	uint16_t offset;
	uint16_t x, y;
	uint16_t width, height;
	int16_t _src_skip;
	uint8_t i;

	btwoGfx_t *dun;
	oldGfxSZ_t *sz;
	oldGfxXY_t *xy;
	btstring_t *data;

	switch (facet) {
	case DUN_FRONT:
		dun = &dun_frontGfx[dunFrontQuadMap[quad]];
		sz = &dunFrontSZ[dunFrontQuadMap[quad]];
		xy = &dunFrontXY[dunFrontQuadMap[quad]];
		doorOffset = dunFrontDoorOffset[dunFrontQuadMap[quad]];
		break;
	case DUN_SIDE:
		dun = &dun_sideGfx[dunSideQuadMap[quad]];
		sz = &dunSideSZ[dunSideQuadMap[quad]];
		xy = &dunSideXY[dunSideQuadMap[quad]];
		doorOffset = dunSideDoorOffset[dunSideQuadMap[quad]];
		break;
	case DUN_PORTAL:
		dun = &dun_portalGfx[dunPortQuadMap[quad]];
		sz = &dunPortSZ[dunPortQuadMap[quad]];
		xy = &dunPortXY[dunPortQuadMap[quad]];
		doorOffset = dunPortFloorOffset[dunPortQuadMap[quad]];
		break;
	}

	height = (sz->top - sz->bottom) + 1;
	width = ((sz->right - sz->left) >> 1) + 1;
	_src_skip = (dun->width << 2) - width;

	if (quadrightflag[quad])
		x = 112 - (width << 1) - xy->x_lo;
	else
		x = xy->x_lo;
	y = xy->y_lo;

	for (i = 0; i < 2; i++) {
		img = bta_cell_new(x, y, width, height, 0, NULL);

		offset = (dun->offset + ((sz->left + ((i) ? doorOffset : 0)) >> 1));

		img->gfx = _getDunGfx(offset, _src_skip, img->width, img->height);
		if (quadrightflag[quad])
			flipRight(img->gfx, img->height, img->width);

		img = bta_cell_convert(img);
		bta_toPNG(img, bts_sprintf("tile_dun_%d_%d_%d.png", dunno, quad, i));

		bta_cell_free(img);
	}
}

static uint8_t *facetString[] = { "", "front", "side", "", "portal" };
static void getDunGfx(uint8_t dunno, uint8_t quad)
{
	uint8_t i;
	uint8_t mask = 1;

	for (i = 0; i < nfacets[quad]; i++) {
		while (!(qfacets[quad] & mask))
			mask <<= 1;

		outputDunFacet(dunno, mask, quad);

		mask <<= 1;
	}
}

static btstring_t *_getDunGfx(uint16_t offset, int16_t _src_skip, int16_t width, int16_t height)
{
	btstring_t *rval;
	uint32_t i, j;

	rval = bts_new(height * width);

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			rval->buf[(i * width) + j] = dundata->buf[offset++];
		}
		offset += _src_skip;
	}

	return rval;
}

static void dun_gfxInit(void)
{
	uint8_t counter;
	uint16_t offset;

	counter = 0;
	offset = 0x9a0;

	do {
		dun_frontGfx[counter].width = (dunFrontSZ[counter].right + dunFrontDoorOffset[counter] + 8) >> 3;
		dun_frontGfx[counter].height = dunFrontSZ[counter].top + 1;

		if (counter < 4) {
			dun_frontGfx[counter].offset = offset;
			dun_frontGfx[counter + 4].offset = offset;
			offset += (dun_frontGfx[counter].width * dun_frontGfx[counter].height) << 2;
		}
		counter++;
	} while (counter < 11);
	dun_frontGfx[8].offset = dun_frontGfx[2].offset;
	dun_frontGfx[9].offset = dun_frontGfx[3].offset;
	dun_frontGfx[10].offset = dun_frontGfx[3].offset;

	counter = 0;
	do {
		dun_portalGfx[counter].width = (dunPortSZ[counter].right + dunPortFloorOffset[counter] + 8) >> 3;
		dun_portalGfx[counter].height = dunPortSZ[counter].top + 1;
		dun_portalGfx[counter].offset = offset;
		offset += (dun_portalGfx[counter].width * dun_portalGfx[counter].height) << 2;
		counter++;
	} while (counter < 6);

	counter = 0;
	do {
		dun_sideGfx[counter].width = (dunSideSZ[counter].right + dunSideDoorOffset[counter] + 8) >> 3;
		dun_sideGfx[counter].height = dunSideSZ[counter].top + 1;
		if (counter < 8) {
			dun_sideGfx[counter].offset = offset;
			offset += (dun_sideGfx[counter].width * dun_sideGfx[counter].height) << 2;
		}
		counter++;
	} while (counter < 10);
	dun_sideGfx[8].offset = dun_sideGfx[5].offset;
	dun_sideGfx[9].offset = dun_sideGfx[6].offset;
}

static void outputBackground(uint8_t tileno)
{
	bta_cell_t *img;
	uint8_t *gfxp;
	uint8_t *left, *right;
	uint32_t i, j;

	img = bta_cell_new(0, 0, 56, 88, 0, NULL);
	img->gfx = bts_new(0x1340);

	gfxp = dundata->buf;

	left = img->gfx->buf;
	right = img->gfx->buf + 55;

	for (i = 0; i < 88; i++) {
		for (j = 0; j < 28; j++) {
			*left++ = *gfxp;
			*right-- = ((*gfxp & 0xf0) >> 4) | ((*gfxp & 0x0f) << 4);
			gfxp++;
		}
		left += 28;
		right = left + 55;
	}

	img = bta_cell_convert(img);
	bta_toPNG(img, bts_sprintf("tile_dun_%d_bg.png", tileno));
	bta_cell_free(img);
}

/****************************************/
/*					*/
/* Public functions			*/
/*					*/
/****************************************/

void outputDunpics(uint8_t indent)
{
	int fd;
	huffile_t *huf;
	int i, j;

	for (j = 0; j < 3; j++) {
		uint8_t fbuf[512];

		sprintf(fbuf, "dpics%d", j);
		fd = xopen(fbuf, O_RDONLY);
		huf = dehuf_init(fd);
		dundata = dehuf(huf, 0x7fff);
		dehuf_free(huf);
		close(fd);

		dun_gfxInit();

		outputBackground(j);

		for (i = 0; i < 24; i++) 
			getDunGfx(j, i);

		bts_free(dundata);
	}
}
