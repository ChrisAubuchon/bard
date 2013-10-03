#include <b1lib.h>
#include <gfx.h>
#include <cnv_gfx.h>
#include <dehuf.h>

#define DEBUG
#include <debug.h>

#define BLDG_FRONT 0x1
#define BLDG_FACE  0x2
#define BLDG_SIDE  0x4

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/
static void city_gfxInit(void);
static void getCityGfx(bt_view_t *view, uint8_t quad);
static void outputCityFacet(bt_view_t *view, uint8_t facet, uint8_t quad);
static void outputBldgFronts(bt_view_t *view);
static void outputBackground(void);

bta_cell_t *getImage(view_t *view, btstring_t *data);

extern uint8_t *bldgFace[];

/****************************************/
/*					*/
/* Local variables			*/
/*					*/
/****************************************/

static btstring_t *piczero;
static btstring_t *picone;

/****************************************/
/*					*/
/* Static variables			*/
/*					*/
/****************************************/

static uint8_t quadmap[] = { QUAD_4L,
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
	0,
};

static uint8_t *animstr[] = {
	"base", "1", "2", "3"
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
/* 0*/ BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE,
/* 2*/ BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE,
/* 4*/ BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE,
/* 6*/ BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE,
/* 8*/ BLDG_FRONT | BLDG_SIDE,
/* 9*/ BLDG_FRONT, BLDG_FRONT, BLDG_FRONT, BLDG_FRONT,
/*13*/ BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE,
/*15*/ BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE,
/*17*/ BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE,
/*19*/ BLDG_FRONT | BLDG_SIDE, BLDG_FRONT | BLDG_SIDE,
/*21*/ BLDG_FRONT | BLDG_SIDE,
/*22*/ BLDG_FRONT, BLDG_FRONT, BLDG_FRONT, BLDG_FRONT,
/*26*/ BLDG_FACE, BLDG_FACE, BLDG_FACE, BLDG_FACE,
/*30*/ BLDG_FACE, BLDG_FACE, BLDG_FACE, BLDG_FACE,
/*34*/ BLDG_FACE
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
	{{{3, 0, 12, 11}, {19, 0, 28, 11}, {35, 0, 44, 11}, {51, 0, 60, 11}}},
	{{{1, 0, 14, 15}, {17, 0, 30, 15}, {33, 0, 46, 15}, {49, 0, 62, 15}}},
	{{{6, 0, 25, 19}, {38, 0, 57, 19}, {70, 0, 89, 19}, {102, 0, 121, 19}}},
	{{{4, 0, 27, 22}, {36, 0, 59, 22}, {68, 0, 91, 22}, {100, 0, 123, 22}}},
	{{{3, 0, 28, 24}, {35, 0, 60, 24}, {67, 0, 92, 24}, {99, 0, 124, 24}}},
	{{{0, 0, 31, 29}, {32, 0, 63, 29}, {64, 0, 95, 29}, {96, 0, 127, 29}}},
	{{{3, 0, 42, 35}, {51, 0, 90, 35}, {99, 0, 138, 35}, {147, 0, 186, 35}}},
	{{{0, 0, 111, 87}, {112, 0, 223, 87}, {224, 0, 335, 87}, {336, 0, 447, 87}}}
};

static bldgGfx_t bldgs_front[] = {
	{{{6, 0, 10, 17}, {14, 0, 18, 17}, {22, 0, 26, 17}, {30, 0, 34, 17}}},
	{{{4, 0, 10, 22}, {12, 0, 18, 22}, {20, 0, 26, 22}, {28, 0, 34, 22}}},
	{{{3, 0, 10, 25}, {11, 0, 18, 25}, {19, 0, 26, 25}, {27, 0, 34, 25}}},
	{{{0, 0, 8, 30}, {16, 0, 24, 30}, {32, 0, 40, 30}, {48, 0, 56, 30}}},
	{{{5, 0, 13, 36}, {21, 0, 29, 36}, {37, 0, 45, 36}, {53, 0, 61, 36}}},
	{{{2, 0, 11, 42}, {18, 0, 27, 42}, {34, 0, 43, 42}, {50, 0, 59, 42}}},
	{{{7, 0, 18, 47}, {23, 0, 34, 47}, {39, 0, 50, 47}, {55, 0, 66, 47}}},
	{{{6, 0, 23, 62}, {30, 0, 47, 62}, {54, 0, 71, 62}, {78, 0, 95, 62}}},
	{{{0, 0, 20, 73}, {24, 0, 44, 73}, {48, 0, 68, 73}, {72, 0, 92, 73}}},
	{{{0, 0, 33, 87}, {40, 0, 73, 87}, {80, 0, 113, 87}, {120, 0, 153, 87}}},
	{{{0, 0, 30, 87}, {32, 0, 62, 87}, {64, 0, 94, 87}, {96, 0, 126, 87}}},
	{{{0, 0, 21, 87}, {24, 0, 45, 87}, {48, 0, 69, 87}, {72, 0, 93, 87}}},
	{{{0, 0, 15, 87}, {16, 0, 31, 87}, {32, 0, 47, 87}, {48, 0, 63, 87}}}
};

static bldgGfx_t bldgs_side[] = {
	{{{0, 0, 23, 24}, {24, 0, 47, 24}, {48, 0, 71, 24}, {72, 0, 95, 24}}},
	{{{0, 0, 31, 27}, {32, 0, 63, 27}, {64, 0, 95, 27}, {96, 0, 127, 27}}},
	{{{0, 0, 39, 29}, {40, 0, 79, 29}, {80, 0, 119, 29}, {120, 0, 159, 29}}},
	{{{0, 0, 31, 36}, {32, 0, 63, 36}, {64, 0, 95, 36}, {96, 0, 127, 36}}},
	{{{0, 0, 39, 43}, {40, 0, 79, 43}, {80, 0, 119, 43}, {120, 0, 159, 43}}},
	{{{0, 0, 39, 49}, {40, 0, 79, 49}, {80, 0, 119, 49}, {120, 0, 159, 49}}},
	{{{0, 0, 31, 55}, {32, 0, 63, 55}, {64, 0, 95, 55}, {96, 0, 126, 55}}},
	{{{0, 0, 23, 68}, {24, 0, 47, 68}, {72, 0, 94, 68}, {72, 0, 94, 68}}},
	{{{0, 0, 15, 76}, {16, 0, 31, 76}, {32, 0, 47, 76}, {48, 0, 62, 76}}}
};

static oldGfxXY_t bldgSideXYOffset[] = {
	{24, 28, 47, 52},
	{16, 26, 47, 53},
	{8, 25, 47, 54},
	{8, 21, 39, 57},
	{0, 16, 39, 59},
	{0, 13, 39, 62},
	{0, 9, 31, 64},
	{0, 2, 23, 70},
	{0, 0, 15, 76}
};

static oldGfxXY_t bldgFrontXYOffset[] = {
	{46, 32, 50, 49},
	{44, 31, 50, 53},
	{43, 28, 50, 53},
	{40, 26, 48, 57},
	{37, 22, 45, 58},
	{34, 20, 43, 62},
	{31, 17, 42, 64},
	{22, 8, 39, 70},
	{16, 2, 36, 75},
	{0, 0, 33, 87},
	{0, 0, 30, 87},
	{0, 0, 21, 87},
	{0, 0, 15, 87}
};

static oldGfxXY_t bldgFaceXYOffset[] = {
	{51, 37, 60, 48},
	{49, 34, 62, 49},
	{46, 32, 65, 51},
	{44, 31, 67, 53},
	{43, 30, 68, 54},
	{40, 27, 71, 56},
	{35, 24, 74, 59},
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

bta_cell_t *getImage(view_t *view, btstring_t *data)
{
	uint16_t x, y;
	uint16_t width, height;
	int16_t src_skip;
	uint16_t offset;

	bta_cell_t *rval;

	uint16_t left_skip = 0;
	uint16_t right_skip = 0;

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

	if ((view->left & 1) == (view->x & 1)) {
		if (view->left & 1)
			left_skip = 1;
		if (!(view->right & 1))
			right_skip = 1;

		rval = bta_trim(rval, left_skip, right_skip);
	} else {
		if (!(view->right & 1)) {
			right_skip = 1;
		}
		/*
		 * Added during BT2 conversion. It removes a
		 * black line from some images.
		 */
		rval = bta_trim(rval, left_skip, right_skip);
	}

	if (view->rflag == 1)
		flipRight(rval->gfx, rval->height, rval->width);

	return bta_cell_scale(rval);
}

static uint8_t *facetString[] = { "", "front", "face", "", "side" };
static uint8_t *sideString[] = { "left", "right", "" };
static void outputCityFacet(bt_view_t *oview, uint8_t facet, uint8_t quad)
{
	uint8_t dist;
	uint8_t i;
	int16_t _src_skip;
	uint16_t offset;
	uint16_t x, y;
	uint16_t height, width;

	btwoGfx_t *city;
	oldGfxSZ_t *sz;
	oldGfxXY_t *xy;
	btstring_t *data;
	view_t view;

	bta_cell_t *img;

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

	height = (sz->top - sz->bottom) + 1;
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

		if (i == 0) {
			btstring_t *q;
			btstring_t *label;
			uint16_t imgx;

			xmkdir(mkImagePath("citypics/%s-%d", 
						animstr[qanimmap[quad]],
						qdepthmap[quad]));
			xmkdir(mkImagePath("citypics/%s-%d/%s%s",
				animstr[qanimmap[quad]],
				qdepthmap[quad],
				sideString[quadrightflag[quad]],
				facetString[facet]));

			q = bts_sprintf("%s-%d", animstr[qanimmap[quad]],
						qdepthmap[quad]),
			label = bts_sprintf("%s%s", 	\
					sideString[quadrightflag[quad]],
					facetString[facet]);

			if (quadrightflag[quad] == 1) 
				imgx = 224 - (img->width) - (xy->x_lo << 1);
			else
				imgx = xy->x_lo << 1;

			bt_view_new_facet(oview, q, label, imgx,
					xy->y_lo << 1, img->width, img->height);
		}
			

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
	bta_cell_t *b;
	uint8_t i;

	xmkdir(mkImagePath("citypics/base-1"));
	xmkdir(mkImagePath("citypics/base-1/face"));

	for (i = 0; i < 4; i++) {
		b = bta_cell_new(0, 0, 56, 88, 0, 
			dehufFile(mkBardOnePath("b%d.huf", i), 0x1340));
		b = bta_cell_convert(b);
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
		counter++;
	} while (counter < 13);

	counter = 0;

	do {
		bldg_sideGfx[counter].offset = offset;
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

	bg = bta_cell_new(0,0,56,88,0,b);
	bg = bta_cell_convert(bg);

	bta_toPNG(bg, mkImagePath("citypics/citybg.png"));

	bta_cell_free(bg);
}

/****************************************/
/*					*/
/* Public functions			*/
/*					*/
/****************************************/

void outputCitypics(uint8_t indent)
{
	FILE *fp;
	huffile_t *huf;
	int i;
	bt_view_t *view;

	xmkdir(mkImagePath("citypics"));

	outputBackground();

	fp = xfopen(mkBardOnePath("citypics.bin"), "rb");
	huf = dehuf_init(fp);

	piczero = dehuf(huf, 0xe0cc);
	picone = dehuf(huf, 0x298c);

	fclose(fp);

	city_gfxInit();

	view = bt_view_new();

	for (i = 0; i < 34; i++) {
		debug("quad: %2d\n", i);
		getCityGfx(view, i);
	}

	outputBldgFronts(view);

	bt_view_to_json(view, mkJsonPath("cityview.json"));

	bt_view_free(view);

	bts_free(piczero);
	bts_free(picone);
	dehuf_free(huf);
}
