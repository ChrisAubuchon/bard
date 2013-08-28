#include <b1lib.h>
#include <gfx.h>
#include <cnv_gfx.h>
#include <dehuf.h>

#define DEBUG
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

static btstring_t *getBackground(void);
static void outputBackground(uint8_t tileset);
static void getDunGfx(bt_view_t *view, uint8_t tileset);
static void getDunFacet(bt_view_t *oview, uint8_t tilset, dunfacet_t *fxxx, uint8_t facet);

/****************************************/
/*					*/
/* Static variables			*/
/*					*/
/****************************************/

static dunfacet_t dunSide[] = {
	{ "FL", "left",  5, 0, 9 },
	{ "L" , "left",  5, 0, 6 },
	{ "M" , "left",  5, 0, 4 },
	{ "FL", "left",  4, 0, 8 },
	{ "L" , "left",  4, 0, 5 },
	{ "M" , "left",  4, 0, 3 },
	{ "L" , "left",  3, 0, 7 },
	{ "M" , "left",  3, 0, 2 },
	{ "M" , "left",  2, 0, 1 },
	{ "M" , "left",  1, 0, 0 },
	{ "FR", "right", 5, 1, 9 },
	{ "R" , "right", 5, 1, 6 },
	{ "M" , "right", 5, 1, 4 },
	{ "FR", "right", 4, 1, 8 },
	{ "R" , "right", 4, 1, 5 },
	{ "M" , "right", 4, 1, 3 },
	{ "R" , "right", 3, 1, 7 },
	{ "M" , "right", 3, 1, 2 },
	{ "M" , "right", 2, 1, 1 },
	{ "M" , "right", 1, 1, 0 },
	{ ""  , "",      0, 0, 0 }
};

static dunfacet_t dunFront[] = { 
	{ "WL", "", 4, 0, 10},
	{ "FL", "", 4, 0, 9 },
	{ "L" , "", 4, 0, 7 },
	{ "FL", "", 3, 0, 8 },
	{ "L" , "", 3, 0, 6 },
	{ "L" , "", 2, 0, 5 },
	{ "L" , "", 1, 0, 4 },
	{ "WR", "", 4, 1, 10},
	{ "FR", "", 4, 1, 9 },
	{ "R" , "", 4, 1, 7 },
	{ "FR", "", 3, 1, 8 },
	{ "R" , "", 3, 1, 6 },
	{ "R" , "", 2, 1, 5 },
	{ "R" , "", 1, 1, 4 },
	{ "M" , "", 4, 0, 3 },
	{ "M" , "", 3, 0, 2 },
	{ "M" , "", 2, 0, 1 },
	{ "M" , "", 1, 0, 0 },
	{ ""  , "", 0, 0, 0 }
};

static dunfacet_t dunPortal[] = {
	{ "L" , "", 4, 0, 5 },
	{ "L" , "", 3, 0, 4 },
	{ "L" , "", 2, 0, 3 },
	{ "R" , "", 4, 1, 5 },
	{ "R" , "", 3, 1, 4 },
	{ "R" , "", 2, 1, 3 },
	{ "M" , "", 4, 0, 2 },
	{ "M" , "", 3, 0, 1 },
	{ "M" , "", 2, 0, 0 },
	{ ""  , "", 0, 0, 0 }
};

static btwoGfx_t dun_frontGfx[11];
static btwoGfx_t dun_portalGfx[6];
static btwoGfx_t dun_sideGfx[10];

static btstring_t *dundata;

static oldGfxXY_t dunFrontXY[] = {
	{  9,  0, 103, 85 },
	{ 29, 13,  83, 66 },
	{ 40, 24,  72, 56 },
	{ 48, 33,  64, 49 },
	{  0,  0,   8, 85 },
	{  0, 13,  28, 66 },
	{  7, 24,  39, 56 },
	{ 31, 33,  47, 49 },
	{  0, 24,   6, 56 },
	{ 14, 33,  30, 49 },
	{  0, 33,  13, 49 }
};

static oldGfxSZ_t dunFrontSZ[] = {
	{  1, 0, 95, 85 },
	{  5, 0, 59, 53 },
	{  0, 0, 32, 32 },
	{  0, 0, 16, 16 },
	{ 87, 0, 95, 85 },
	{ 31, 0, 59, 53 },
	{  0, 0, 32, 32 },
	{  0, 0, 16, 16 },
	{ 26, 0, 32, 32 },
	{  0, 0, 16, 16 },
	{  3, 0, 16, 16 }
};

static uint16_t dunFrontDoorOffset[] = {
	96, 56, 40, 24, 96, 56, 40, 24, 40, 24, 24
};

static oldGfxXY_t dunPortXY[] = {
	{ 48, 72, 63, 78 },
	{ 48, 61, 63, 63 },
	{ 48, 52, 63, 53 },
	{  0, 72,  7, 77 },
	{  8, 60, 23, 62 },
	{ 24, 52, 39, 53 }
};

static oldGfxSZ_t dunPortSZ[] = {
	{ 0, 0, 15, 6 }, 
	{ 0, 0, 15, 2 },
	{ 0, 0, 15, 1 },
	{ 0, 0,  7, 5 },
	{ 0, 0, 15, 2 },
	{ 0, 0, 15, 1 }
};

static uint16_t dunPortFloorOffset[] = {
	16, 16, 16, 8, 16, 16
};

static oldGfxXY_t dunSideXY[] = {
	{  0,  0,  8, 87 },
	{  9,  0, 28, 84 },
	{ 29, 14, 39, 65 },
	{ 40, 25, 47, 55 },
	{ 48, 34, 51, 48 },
	{  8, 24, 29, 55 },
	{ 30, 32, 42, 49 },
	{  0, 21,  7, 60 },
	{  0, 24, 13, 55 },
	{ 14, 32, 26, 49 }
};

static oldGfxSZ_t dunSideSZ[] = {
	{ 0, 0,  8, 87 },
	{ 1, 0, 20, 84 },
	{ 5, 0, 15, 51 },
	{ 0, 0,  7, 30 },
	{ 0, 0,  3, 14 },
	{ 0, 0, 21, 31 },
	{ 6, 0, 18, 17 },
	{ 0, 0,  7, 39 },
	{ 8, 0, 21, 31 },
	{ 6, 0, 18, 17 }
};

static uint16_t dunSideDoorOffset[] = {
	16, 24, 16, 8, 8, 24, 16, 8, 24, 16
};

/****************************************/
/*					*/
/* Local functions			*/
/*					*/
/****************************************/

static void getDunFacet(bt_view_t *outView, uint8_t tileset, dunfacet_t *fxxx, uint8_t facet)
{
	uint16_t doorOffset = 0;
	uint16_t x, y;
	uint16_t offset;
	uint8_t i;

	btwoGfx_t *dun;
	oldGfxSZ_t *sz;
	oldGfxXY_t *xy;
	btstring_t *data;
	view_t view;

	bta_cell_t *img;

	switch (facet) {
	case DUN_FRONT:
		dun = &dun_frontGfx[fxxx->faceMap];
		sz = &dunFrontSZ[fxxx->faceMap];
		xy = &dunFrontXY[fxxx->faceMap];
		doorOffset = dunFrontDoorOffset[fxxx->faceMap];
		break;
	case DUN_SIDE:
		dun = &dun_sideGfx[fxxx->faceMap];
		sz = &dunSideSZ[fxxx->faceMap];
		xy = &dunSideXY[fxxx->faceMap];
		doorOffset = dunSideDoorOffset[fxxx->faceMap];
		break;
	case DUN_PORTAL:
		dun = &dun_portalGfx[fxxx->faceMap];
		sz = &dunPortSZ[fxxx->faceMap];
		xy = &dunPortXY[fxxx->faceMap];
		doorOffset = dunPortFloorOffset[fxxx->faceMap];
		break;
	}

	view.x = xy->x_lo;
	view.y = xy->y_lo;
	view.rflag = fxxx->rightFlag;
	view.left = sz->left;
	view.right = sz->right;
	view.top = sz->top;
	view.bottom = sz->bottom;
	view.width = dun->width;

	for (i = 0; i < 2; i++) {
		btstring_t *label;
		btstring_t *l;

		view.right = sz->right + ((i) ? doorOffset : 0);
		view.left = sz->left + ((i) ? doorOffset : 0);
		view.offset = dun->offset;

		img = getImage(&view, dundata);

		switch (facet) {
		case DUN_FRONT:
			label = bts_strcpy("front");
			break;
		case DUN_SIDE:
			label = bts_sprintf("%sside", fxxx->extra);
			break;
		case DUN_PORTAL:
			label = bts_strcpy("portal");
			break;
		}

		if (i == 0) {
			btstring_t *q;
			uint16_t imgx;

			debug("label->buf = %s\n", label->buf);
			xmkdir(mkImagePath("dpics/%d-%s", fxxx->depth,
							fxxx->name));
			xmkdir(mkImagePath("dpics/%d-%s/%s",
						fxxx->depth,
						fxxx->name,
						label->buf));
			q = bts_sprintf("%d-%s", fxxx->depth, 
						fxxx->name
					);

			if (fxxx->rightFlag == 1)
				imgx = 224 - (img->width) - (xy->x_lo << 1);
			else
				imgx = xy->x_lo << 1;

			bt_view_new_facet(outView, q, label, imgx,
						xy->y_lo << 1,
						img->width, img->height);
		}

		if (i) {
			if (facet == DUN_PORTAL) 
				l = bts_strcpy("ceiling");
			else
				l = bts_strcpy("wall");
		} else {
			if (facet == DUN_PORTAL)
				l = bts_strcpy("floor");
			else
				l = bts_strcpy("door");
		}
		bta_toPNG(img,
			mkImagePath("dpics/%d-%s/%s/%d-%s.png",
				fxxx->depth, fxxx->name,
				label->buf, tileset, l->buf));
		bts_free(l);
		bta_cell_free(img);
	}
}

static void getDunGfx(bt_view_t *view, uint8_t tileset)
{
	uint8_t i;
	uint8_t mask = 1;

	i = 0;
	while (dunSide[i].depth) {
		getDunFacet(view, tileset, &dunSide[i], DUN_SIDE);
		i++;
	}
	i = 0;
	while (dunFront[i].depth) {
		getDunFacet(view, tileset, &dunFront[i], DUN_FRONT);
		i++;
	}
	i = 0;
	while (dunPortal[i].depth) {
		getDunFacet(view, tileset, &dunPortal[i], DUN_PORTAL);
		i++;
	}
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
		debug("dun_portalGfx[%d].offset = %x\n", counter, offset);
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

static void outputBackground(uint8_t tileset)
{
	bta_cell_t *bg;
	btstring_t *rval;
	uint8_t *gfxp;
	uint8_t *left, *right;
	uint32_t i, j;

	gfxp = dundata->buf;
	rval = bts_new(0x1340);

	left = rval->buf;
	right = rval->buf + 55;

	for (i = 0; i < 88; i++) {
		for (j = 0; j < 28; j++) {
			*left++ = *gfxp;
			*right-- = ((*gfxp & 0xf0) >> 4) | ((*gfxp & 0x0f) << 4);
			gfxp++;
		}
		left += 28;
		right = left + 55;
	}

	bg = bta_cell_new(0,0,56,88,0, rval);
	bg = bta_cell_convert(bg);
	bta_toPNG(bg, mkImagePath("dpics/%d-bg.png", tileset));
	bta_cell_free(bg);

}

/****************************************/
/*					*/
/* Public functions			*/
/*					*/
/****************************************/

void outputDunpics(uint8_t indent)
{
	huffile_t *huf;
	FILE *fp;
	int i, j;
	bt_view_t *view;
	
	xmkdir(mkImagePath("dpics"));

	for (i = 0; i < 3; i++) {
		fp = xfopen(mkBardOnePath("dpics%d", i), "rb");
		huf = dehuf_init(fp);
		dundata = dehuf(huf, 0x7fff);
		dehuf_free(huf);

		outputBackground(i);
		dun_gfxInit();

		view = bt_view_new();
		getDunGfx(view, i);

		bt_view_to_json(view, mkJsonPath("dunview.json"));
		bt_view_free(view);
		bts_free(dundata);
	}
}
