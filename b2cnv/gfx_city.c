#include <b2lib.h>
#include <gfx.h>
#include <cnv_gfx.h>
#include <dehuf.h>

/*#define DEBUG*/
#include <debug.h>

#define BLDG_FRONT 0x1
#define BLDG_FACE  0x2
#define BLDG_SIDE  0x4

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/

static void outputBackground(void);
static void city_gfxInit(void);
static void getCityGfx(bt_view_t *view, uint8_t quad);
static void outputCityFacet(bt_view_t *oview, uint8_t facet, uint8_t quad);
static void outputBldgFronts(bt_view_t *view);

/****************************************/
/*					*/
/* Local variables			*/
/*					*/
/****************************************/

static btstring_t *piczero;
static btstring_t *picone;


bta_color_t cityPalette[16] = {
	{   0,   0,   0, 255 }, /*0*/
	{   0,   0, 170, 255 }, /*1*/
	{   0, 170,   0, 255 }, /*2*/
	{   0, 170, 170, 255 }, /*3*/
	{ 170,   0,   0, 255 }, /*4*/
	{ 170,   0, 170, 255 }, /*5*/
	{ 170,  85,   0, 255 }, /*6*/
	{ 170, 170, 170, 255 }, /*7*/
	{  85,  85,  85, 255 }, /*8*/
	{  85,  85, 255, 255 }, /*9*/
	{  85, 255,  85, 255 }, /*a*/
	{  85, 255, 255, 0   }, /*b*/
	{ 255,  85,  85, 255 }, /*c*/
	{ 255,  85, 255, 255 }, /*d*/
	{ 255, 255,  85, 255 }, /*e*/
	{ 255, 255, 255, 255 }  /*f*/
};

/****************************************/
/*					*/
/* Static variables			*/
/*					*/
/****************************************/

#if 0
static uint8_t quadmap[] = { 
	QUAD_4L,
	QUAD_3L, QUAD_3L, QUAD_3L, QUAD_3L,
	QUAD_2L, QUAD_2L, QUAD_2L, QUAD_2L,
	QUAD_1L, QUAD_1L, QUAD_1L, QUAD_1L,
	QUAD_4R,
	QUAD_3R, QUAD_3R, QUAD_3R, QUAD_3R,
	QUAD_2R, QUAD_2R, QUAD_2R, QUAD_2R,
	QUAD_1R, QUAD_1R, QUAD_1R, QUAD_1R,
	QUAD_4M, QUAD_4M, QUAD_4M, QUAD_4M,
	QUAD_3M, QUAD_3M, QUAD_3M, QUAD_3M,
	QUAD_2M
};
#endif

static uint8_t qanimmap[] = { 
	3,
	0, 1, 2, 3,
	0, 1, 2, 3,
	0, 1, 2, 3,
	3,
	0, 1, 2, 3,
	0, 1, 2, 3,
	0, 1, 2, 3,
	0, 1, 2, 3,
	0, 1, 2, 3,
	0
};

static uint8_t *animstr[] = {
	"base", "1", "2", "3"
};

uint8_t *bldgFace[] = {
	"",
	"building1",
	"building2",
	"building3",
	"building4"
};

static uint8_t qdepthmap[] = {
	4,
	3, 3, 3, 3,
	2, 2, 2, 2,
	1, 1, 1, 1,
	4,
	3, 3, 3, 3,
	2, 2, 2, 2,
	1, 1, 1, 1,
	3, 3, 3, 3,
	2, 2, 2, 2,
	1
};

static uint8_t qfacets[] = {
	BLDG_FRONT | BLDG_SIDE, 
	BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE, 
	BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE,
	BLDG_FRONT, BLDG_FRONT, BLDG_FRONT, BLDG_FRONT, 
	BLDG_FRONT | BLDG_SIDE, 
	BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE,
	BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE,
	BLDG_FRONT, BLDG_FRONT, BLDG_FRONT, BLDG_FRONT,
	BLDG_FACE, BLDG_FACE, BLDG_FACE, BLDG_FACE,
	BLDG_FACE, BLDG_FACE, BLDG_FACE, BLDG_FACE,
	BLDG_FACE
};

static uint8_t nfacets[] = {
/* 0*/ 2, 2,
/* 2*/ 2, 2,
/* 4*/ 2, 2,
/* 6*/ 2, 2,
/* 8*/ 2,
/* 9*/ 1, 1, 1, 1,
/*13*/ 2, 2,
/*15*/ 2, 2,
/*17*/ 2, 2,
/*19*/ 2, 2,
/*21*/ 2,
/*22*/ 1, 1, 1, 1,
/*26*/ 1, 1, 1, 1,
/*30*/ 1, 1, 1, 1,
/*34*/ 1
};

static uint8_t quadrightflag[] = {
	0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	1, 1, 1, 1,
	2, 2, 2, 2,
	2, 2, 2, 2,
	2
};

static uint8_t quaddistmap[] = {
	0,
	1, 2, 3, 4,
	5, 6, 7, 8,
	9, 10, 11, 12,
	0,
	1, 2, 3, 4,
	5, 6, 7, 8,
	9, 10, 11, 12,
	0, 0, 1, 2, 
	3, 4, 5, 6,
	0xff
};

static bldgGfx_t bldgs_face[] = {
	{{{2, 0, 14, 11}, {18, 0, 30, 11}, {34, 0, 46, 11}, {50, 0, 62, 11}}},
	{{{0, 0, 18, 18}, {24, 0, 42, 18}, {48, 0, 66, 18}, {72, 0, 90, 18}}},
	{{{6, 0, 27, 22}, {38, 0, 59, 22}, {70, 0, 91, 22}, {102, 0, 123, 22}}},
	{{{3, 0, 30, 25}, {35, 0, 62, 25}, {67, 0, 94, 25}, {99, 0, 126, 25}}},
	{{{0, 0, 33, 31}, {40, 0, 73, 31}, {80, 0, 113, 31}, {120, 0, 153, 31}}},
	{{{5, 0, 44, 37}, {53, 0, 92, 37}, {101, 0, 140, 37}, {149, 0, 188, 37}}},
	{{{2, 0, 47, 43}, {50, 0, 95, 43}, {98, 0, 143, 43}, {146, 0, 191, 43}}},
	{{{0, 0, 111, 87}, {112, 0, 223, 87}, {224, 0, 335, 87}, {336, 0, 447, 87}}}
};

static bldgGfx_t bldgs_front[] = {
	{{{6, 0, 10, 17}, {14, 0, 18, 17}, {22, 0, 26, 17}, {30, 0, 34, 17}}},
	{{{4, 0, 8, 22}, {12, 0, 16, 22}, {20, 0, 24, 22}, {28, 0, 32, 22}}},
	{{{3, 0, 9, 25}, {11, 0, 17, 25}, {19, 0, 25, 25}, {27, 0, 33, 25}}},
	{{{0, 0, 8, 31}, {16, 0, 24, 31}, {32, 0, 40, 31}, {48, 0, 56, 31}}},
	{{{5, 0, 15, 37}, {21, 0, 31, 37}, {37, 0, 47, 37}, {53, 0, 63, 37}}},
	{{{3, 0, 15, 43}, {18, 0, 30, 43}, {34, 0, 46, 43}, {50, 0, 62, 43}}},
	{{{7, 0, 21, 50}, {23, 0, 37, 50}, {39, 0, 53, 50}, {55, 0, 69, 50}}},
	{{{6, 0, 25, 66}, {30, 0, 49, 66}, {54, 0, 73, 66}, {78, 0, 97, 66}}},
	{{{0, 0, 23, 80}, {24, 0, 47, 80}, {48, 0, 71, 80}, {72, 0, 95, 80}}},
	{{{0, 0, 33, 87}, {40, 0, 73, 87}, {80, 0, 113, 87}, {120, 0, 153, 87}}},
	{{{0, 0, 30, 87}, {32, 0, 62, 87}, {64, 0, 94, 87}, {96, 0, 126, 87}}},
	{{{0, 0, 21, 87}, {24, 0, 45, 87}, {48, 0, 69, 87}, {72, 0, 93, 87}}},
	{{{0, 0, 15, 87}, {16, 0, 31, 87}, {32, 0, 47, 87}, {48, 0, 63, 87}}}
};

static bldgGfx_t bldgs_side[] = {
	{{{8, 0, 31, 17}, {40, 0, 63, 17}, {74, 0, 95, 17}, {104, 0, 127, 17}}},
	{{{4, 0, 31, 21}, {36, 0, 63, 21}, {68, 0, 95, 21}, {100, 0, 127, 21}}},
	{{{5, 0, 39, 25}, {45, 0, 79, 25}, {86, 0, 119, 25}, {125, 0, 159, 25}}},
	{{{7, 0, 47, 31}, {55, 0, 95, 31}, {103, 0, 143, 31}, {152, 0, 191, 31}}},
	{{{0, 0, 39, 37}, {40, 0, 79, 37}, {80, 0, 119, 37}, {120, 0, 159, 37}}},
	{{{0, 0, 39, 44}, {40, 0, 79, 44}, {80, 0, 119, 44}, {120, 0, 159, 44}}},
	{{{0, 0, 31, 51}, {32, 0, 63, 51}, {64, 0, 95, 51}, {96, 0, 126, 51}}},
	{{{0, 0, 23, 66}, {24, 0, 47, 66}, {48, 0, 71, 66}, {72, 0, 94, 66}}},
	{{{0, 0, 15, 80}, {16, 0, 31, 80}, {32, 0, 47, 80}, {48, 0, 62, 80}}}
};

static oldGfxXY_t bldgSideXYOffset[] = {
	{24, 35, 47, 50},
	{20, 32, 47, 52},
	{13, 29, 47, 54},
	{8, 26, 47, 57},
	{0, 23, 39, 60},
	{0, 19, 39, 63},
	{0, 16, 31, 67},
	{0, 9, 23, 75},
	{0, 2, 15, 82}
};

static oldGfxXY_t bldgFrontXYOffset[] = {
	{46, 35, 50, 50},
	{45, 32, 49, 53},
	{44, 29, 50, 54},
	{40, 26, 48, 57},
	{35, 23, 45, 60},
	{32, 20, 44, 63},
	{29, 17, 43, 67},
	{20, 9, 39, 75},
	{13, 2, 36, 82},
	{0, 0, 33, 87},
	{0, 0, 30, 87},
	{0, 0, 21, 87},
	{0, 0, 15, 87}
};

static oldGfxXY_t bldgFaceXYOffset[] = {
	{50, 38, 62, 49},
	{48, 32, 66, 50},
	{46, 30, 67, 52},
	{43, 29, 70, 54},
	{40, 26, 73, 57},
	{37, 23, 76, 60},
	{34, 20, 79, 63},
	{0, 0, 111, 87}
};

static btwoGfx_t bldg_frontGfx[13];
static btwoGfx_t bldg_sideGfx[9];
static btwoGfx_t bldg_faceGfx[8];

/****************************************/
/*					*/
/* Local functions			*/
/*					*/
/****************************************/

static uint8_t *facetString[] = { "", "front", "face", "", "side" };
static uint8_t *sideString[] = { "left", "right", "" };

static void outputCityFacet(bt_view_t *oview, uint8_t facet, uint8_t quad)
{
	uint8_t		dist;
	uint8_t		i;
	int16_t		_src_skip;
	uint16_t	offset;
	uint16_t	x, y;
	uint16_t	width;

	btwoGfx_t	*city;
	oldGfxSZ_t	*sz;
	oldGfxXY_t	*xy;
	btstring_t	*data;
	view_t		view;
	bta_cell_t	*img;

	dist = quaddistmap[quad];

	switch (facet) {
	case BLDG_FRONT:
		sz = &bldgs_front[dist].blist[0];
		xy = &bldgFrontXYOffset[dist];
		data = piczero;
		break;
	case BLDG_SIDE:
		sz = &bldgs_side[dist].blist[0];
		xy = &bldgSideXYOffset[dist];
		data = piczero;
		break;
	case BLDG_FACE:
		sz = &bldgs_face[dist].blist[0];
		xy = &bldgFaceXYOffset[dist];
		data = picone;
		break;
	}

	width = ((sz->right >> 1) - (sz->left >> 1)) + 1;

	if (quadrightflag[quad] == 1)
		x = 112 - (width << 1) - xy->x_lo;
	else
		x = xy->x_lo;
	y = xy->y_lo;

	view.x = xy->x_lo;
	view.y = xy->y_lo;
	view.rflag = quadrightflag[quad];

	for (i = 0; i < 4; i++) {
		switch (facet) {
		case BLDG_FRONT:
			city = &bldg_frontGfx[dist];
			sz = &bldgs_front[dist].blist[i];
			break;
		case BLDG_SIDE:
			city = &bldg_sideGfx[dist];
			sz = &bldgs_side[dist].blist[i];
			break;
		case BLDG_FACE:
			city = &bldg_faceGfx[dist];
			sz = &bldgs_face[dist].blist[i];
			break;
		}

		view.left = sz->left;
		view.right = sz->right;
		view.top = sz->top;
		view.bottom = sz->bottom;
		view.width = city->width;
		view.offset = city->offset;

		img = getImage(&view, data);
		img = bta_cell_toRGBA(img, cityPalette);

		if (i == 0) {
			btstring_t	*q;
			btstring_t	*label;
			uint16_t	imgx;

			xmkdir(mkImagePath("citypics/%s-%d",
					animstr[qanimmap[quad]],
					qdepthmap[quad]));
			xmkdir(mkImagePath("citypics/%s-%d/%s%s",
					animstr[qanimmap[quad]],
					qdepthmap[quad],
					sideString[quadrightflag[quad]],
					facetString[facet]
					)
				);
			q = bts_sprintf("%s-%d", 
					animstr[qanimmap[quad]],
					qdepthmap[quad]
					);
			label = bts_sprintf("%s%s",
					sideString[quadrightflag[quad]],
					facetString[facet]
					);
			if (quadrightflag[quad] == 1)
				imgx = 224 - (img->width) - (xy->x_lo << 1);
			else
				imgx = xy->x_lo << 1;

			bt_view_new_facet(oview, q, label, imgx,
				xy->y_lo << 1, img->width, img->height);
		}

		debug("%s-%d/%s%s/%s.png\n\n",
				animstr[qanimmap[quad]],
				qdepthmap[quad],
				sideString[quadrightflag[quad]],
				facetString[facet],
				bldgFace[i+1]
			);
		bta_toPNG(img, 
			mkImagePath("citypics/%s-%d/%s%s/%s.png",
				animstr[qanimmap[quad]],
				qdepthmap[quad],
				sideString[quadrightflag[quad]],
				facetString[facet],
				bldgFace[i+1]
			)
		);

		bta_cell_free(img);
	}

}

static void getCityGfx(bt_view_t *view, uint8_t quad)
{
	uint8_t i;
	uint8_t mask = 1;

	for (i = 0; i < nfacets[quad]; i++) {
		while (!(qfacets[quad] & mask))
			mask <<= 1;

		outputCityFacet(view, mask, quad);
		mask <<= 1;
	}
}

static void outputBldgFronts(bt_view_t *view)
{
	bta_cell_t	*b;
	uint8_t		i;

	xmkdir(mkImagePath("citypics/base-1"));
	xmkdir(mkImagePath("citypics/base-1/face"));

	for (i = 0; i < 4; i++) {
		b = bta_cell_new(0, 0, 56, 88, 0, 
			dehufFile(mkBardTwoPath("B%d.HUF", i), 0x1340));
		b = bta_cell_scale(bta_cell_4bitTo8bit(b));
		b = bta_cell_toRGBA(b, cityPalette);
		bta_toPNG(b, 
			mkImagePath("citypics/base-1/face/%s.png",
				bldgFace[i+1]));
		bta_cell_free(b);
		bt_view_new_facet(view, bts_strcpy("base-1"), \
			bts_strcpy("face"), 0, 0, 224, 176);
	}
}

void flipRight(btstring_t *r, uint8_t height, uint8_t width)
{
	uint8_t tmp;
	uint8_t cnv;
	uint8_t h, w;

	for (h = 0; h < height; h++) {
		for (w = 0; w < (width >> 1); w++) {
			tmp = r->buf[(h * width) + w];
			r->buf[(h * width) + w] = r->buf[(h * width) + (width - w - 1)];
			r->buf[(h * width) + (width - w - 1)] = tmp;
		}
	}
}

static void city_gfxInit(void)
{
	uint16_t offset;
	int counter;

	counter = 0;
	offset = 0;

	do {
		bldg_frontGfx[counter].offset = offset;
		bldg_frontGfx[counter].width = (bldgs_front[counter].blist[3].right + 8) >> 3;
		bldg_frontGfx[counter].height = bldgs_front[counter].blist[3].top + 1;
		offset += (bldg_frontGfx[counter].height * bldg_frontGfx[counter].width) << 2;
		debug("bldg_frontGfx[%d].offset = %x\n", counter, bldg_frontGfx[counter].offset);
		debug("bldg_frontGfx[%d].width = %d\n", counter, bldg_frontGfx[counter].width);
		debug("bldg_frontGfx[%d].height = %d\n", counter, bldg_frontGfx[counter].height);
		counter++;
	} while (counter < 13);

	counter = 0;

	do {
		bldg_sideGfx[counter].offset = offset;
		debug("multiplier = %x\n", bldgs_side[counter].blist[3].right + 8);
		bldg_sideGfx[counter].width = (bldgs_side[counter].blist[3].right + 8) >> 3;
		bldg_sideGfx[counter].height = bldgs_side[counter].blist[3].top + 1;
		offset += (bldg_sideGfx[counter].height * bldg_sideGfx[counter].width) << 2;
		counter++;
	} while (counter < 9);

	offset = 0;
	counter = 0;

	do {
		bldg_faceGfx[counter].offset = offset;
		bldg_faceGfx[counter].width = (bldgs_face[counter].blist[3].right + 8) >> 3;
		bldg_faceGfx[counter].height = bldgs_face[counter].blist[3].top + 1;
		offset += (bldg_faceGfx[counter].height * bldg_faceGfx[counter].width) << 2;
		counter++;
	} while (counter < 8);
}

static void outputBackground(void)
{
	bta_cell_t *bg;
	btstring_t *b;
	uint32_t i;

	b = bts_new(0x1340);
	for (i = 0; i < 2576; i++)
		b->buf[i] = 0xbb;
	for (; i < 4928; i++)
		b->buf[i] = 0xcc;

	bg = bta_cell_new(0, 0, 56, 88, 0, b);
	bg = bta_cell_convert(bg);

	bta_toPNG(bg, mkImagePath("citypics/citybg-day.png"));

	bta_cell_free(bg);

	b = bts_new(0x1340);
	for (i = 0; i < 2576; i++)
		b->buf[i] = 0x0;
	for (; i < 4928; i++)
		b->buf[i] = 0xcc;

	bg = bta_cell_new(0, 0, 56, 88, 0, b);
	bg = bta_cell_convert(bg);

	bta_toPNG(bg, mkImagePath("citypics/citybg-night.png"));

	bta_cell_free(bg);
}

/****************************************/
/*					*/
/* Public functions			*/
/*					*/
/****************************************/

bta_cell_t *getImage(view_t *view, btstring_t *data)
{
	uint16_t	x, y;
	uint16_t	width, height;
	int16_t		src_skip;
	uint16_t	offset;

	uint16_t	left_skip = 0;
	uint16_t	right_skip = 0;

	bta_cell_t	*rval;

	height = (view->top - view->bottom) + 1;
	width = ((view->right >> 1) - (view->left >> 1)) + 1;

	if (view->rflag == 1)
		view->x = 112 - (width << 1) - view->x;

	offset = view->offset + (view->left >> 1);
	src_skip = (view->width << 2) - width;

	rval = bta_cell_new(view->x, view->y, width, height, 0, NULL);
	rval->gfx = bts_new(rval->width * rval->height);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			rval->gfx->buf[(y * rval->width) + x] = data->buf[offset++];
		}
		offset += src_skip;
	}

	rval = bta_cell_4bitTo8bit(rval);

	debug("left  = %3d\n", view->left);
	debug("right = %3d\n", view->right);
	debug("x     = %3d\n", view->x);
	if ((view->left & 1) == (view->x & 1)) {
		if (view->left & 1)
			left_skip = 1;
		if (!(view->right & 1))
			right_skip = 1;

		rval = bta_trim(rval, left_skip, right_skip);
	} else {
		if (!(view->right & 1))
			right_skip = 1;

		rval = bta_trim(rval, left_skip, right_skip);
	}

	if (view->rflag == 1)
		flipRight(rval->gfx, rval->height, rval->width);

	return bta_cell_scale(rval);
}

void outputCitypics(void)
{
	huffile_t	*huf;
	uint32_t	i;
	bt_view_t	*view;
	FILE		*fp;

	xmkdir(mkImagePath("citypics"));

	outputBackground();
	
	fp = xfopen(mkBardTwoPath("CITYPICS.BIN"), "rb");
	huf = dehuf_init(fp);

	piczero = dehuf(huf, 0xe034);
	picone = dehuf(huf, 0x3a10);

	fclose(fp);

	city_gfxInit();

	view = bt_view_new();

	for (i = 0; i < 34; i++) {
		debug("i = %2d\n", i);
		getCityGfx(view, i);
	}

	outputBldgFronts(view);

	bt_view_to_json(view, mkJsonPath("cityview.json"));
	bt_view_free(view);

	bts_free(piczero);
	bts_free(picone);
	dehuf_free(huf);
}
