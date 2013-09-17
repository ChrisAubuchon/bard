#include <b2lib.h>
#include <gfx.h>
#include <cnv_gfx.h>
#include <dehuf.h>

/*#define DEBUG*/
#include <debug.h>

/****************************************/
/*					*/
/* Local macros				*/
/*					*/
/****************************************/
#define WILD_TREE		1
#define WILD_HUT		2
#define WILD_WALL		3
#define	WILD_ENTR		4

#define WILD_FRONT		0x10
#define WILD_SIDE		0x20
#define WILD_CORNER		0x30
#define WILD_EDGE		0x40

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/
static void wild_gfxInit(void);
static void wild_mirrorGfx(btwoGfx_t * fromStruct, btwoGfx_t * toStruct);
static void getWildGfx(bt_view_t *view);
static void outputWildFacet(bt_view_t *oview, dunfacet_t *df, uint8_t facet);

/****************************************/
/*					*/
/* Local variables			*/
/*					*/
/****************************************/
static btwoGfx_t wild_treeGfx[3];
static btwoGfx_t wild_hutGfx[3];
static btwoGfx_t wild_cityWallGfx[3];
static btwoGfx_t wild_cityEntrGfx[3];
static btwoGfx_t wild_wallLtSideGfx[3];
static btwoGfx_t wild_entrLtSideGfx[3];
static btwoGfx_t wild_wallLtCrnrGfx[2];
static btwoGfx_t wild_wallRtSideGfx[3];
static btwoGfx_t wild_entrRtSideGfx[3];
static btwoGfx_t wild_wallRtCrnrGfx[2];

static btstring_t *wpic;

/****************************************/
/*					*/
/* Static variables			*/
/*					*/
/****************************************/

static dunfacet_t dunWall[] = {
	{ "FL", "-edge",	4, 0,  0, WILD_EDGE	},
	{ "L",  "",		4, 0,  0, WILD_FRONT	},
	{ "L",  "-corner",	3, 0,  0, WILD_CORNER	},
	{ "L",  "-side",	3, 0,  0, WILD_SIDE	},
	{ "L",  "",		3, 0,  1, WILD_FRONT	},
	{ "L",  "-corner",	2, 0,  1, WILD_CORNER	},
	{ "L",  "-side",	2, 0,  1, WILD_SIDE	},
	{ "L",  "",		2, 0,  2, WILD_FRONT	},
	{ "L",  "-side",	1, 0,  2, WILD_SIDE	},
	{ "FR", "-edge",	4, 2,  2, WILD_EDGE	},
	{ "R",  "",		4, 2,  0, WILD_FRONT	},
	{ "R",  "-corner",	3, 2,  0, WILD_CORNER	},
	{ "R",  "-side",	3, 2,  0, WILD_SIDE	},
	{ "R",  "",		3, 2,  1, WILD_FRONT	},
	{ "R",  "-corner",	2, 2,  1, WILD_CORNER	},
	{ "R",  "-side",	2, 2,  1, WILD_SIDE	},
	{ "R",  "",		2, 2,  2, WILD_FRONT	},
	{ "R",  "-side",	1, 2,  2, WILD_SIDE	},
	{ "M",  "",		4, 1,  2, WILD_FRONT	},
	{ "M",  "",		3, 1,  1, WILD_FRONT	},
	{ "M",  "",		2, 1,  2, WILD_FRONT	},
	{ "",   "",		0, 0,  0, 0 }
};

static dunfacet_t dunEntr[] = {
	{ "FL", "-edge",	4, 0,  0, WILD_EDGE	},
	{ "L",  "",		4, 0,  0, WILD_FRONT	},
	{ "L",  "-side",	3, 0,  0, WILD_SIDE	},
	{ "L",  "",		3, 0,  1, WILD_FRONT	},
	{ "L",  "-side",	2, 0,  1, WILD_SIDE	},
	{ "L",  "",		2, 0,  2, WILD_FRONT	},
	{ "L",  "-side",	1, 0,  2, WILD_SIDE	},
	{ "FR", "-edge",	4, 2,  0, WILD_EDGE	},
	{ "R",  "",		4, 2,  0, WILD_FRONT	},
	{ "R",  "-side",	3, 2,  0, WILD_SIDE	},
	{ "R",  "",		3, 2,  1, WILD_FRONT	},
	{ "R",  "-side",	2, 2,  1, WILD_SIDE	},
	{ "R",  "",		2, 2,  2, WILD_FRONT	},
	{ "R",  "-side",	1, 2,  2, WILD_SIDE	},
	{ "M",  "",		4, 1,  2, WILD_FRONT	},
	{ "M",  "",		3, 1,  1, WILD_FRONT	},
	{ "M",  "",		2, 1,  2, WILD_FRONT	},
	{ "",   "",		0, 0,  0, 0 }
};

static dunfacet_t dunTreeHut[] = {
	{ "L", "", 4, 0, 0, WILD_FRONT },
	{ "L", "", 3, 0, 1, WILD_FRONT },
	{ "L", "", 2, 0, 2, WILD_FRONT },
	{ "R", "", 4, 2, 0, WILD_FRONT },
	{ "R", "", 3, 2, 1, WILD_FRONT },
	{ "R", "", 2, 2, 2, WILD_FRONT },
	{ "M", "", 4, 1, 0, WILD_FRONT },
	{ "M", "", 3, 1, 1, WILD_FRONT },
	{ "M", "", 2, 1, 2, WILD_FRONT },
	{ "",  "", 0, 0, 0, 0 }
};

#if 0
static uint16_t qfacets[] = {
/*4FL*/ WILD_WALLEDGE | WILD_ENTREDGE,
/* 4L*/ WILD_TREE | WILD_HUT | WILD_WALL | WILD_ENTR,
/* 3L*/ WILD_WALL_LTCRNR | WILD_WALL_LTSIDE | WILD_ENTR_LTSIDE | WILD_TREE | WILD_HUT | WILD_WALL | WILD_ENTR,
/* 2L*/ WILD_WALL_LTCRNR | WILD_WALL_LTSIDE | WILD_ENTR_LTSIDE | WILD_TREE | WILD_HUT | WILD_WALL | WILD_ENTR,
/* 1L*/ WILD_WALL_LTSIDE | WILD_ENTR_LTSIDE,
/*4FR*/ WILD_WALLEDGE | WILD_ENTREDGE,
/* 4R*/ WILD_TREE | WILD_HUT | WILD_WALL | WILD_ENTR, 
/* 3R*/ WILD_WALL_RTCRNR | WILD_WALL_RTSIDE | WILD_ENTR_RTSIDE | WILD_TREE | WILD_HUT | WILD_WALL | WILD_ENTR,
/* 2R*/ WILD_WALL_RTCRNR | WILD_WALL_RTSIDE | WILD_ENTR_RTSIDE | WILD_TREE | WILD_HUT | WILD_WALL | WILD_ENTR,
/* 1R*/ WILD_WALL_RTSIDE | WILD_ENTR_RTSIDE,
/* 4M*/ WILD_TREE | WILD_HUT | WILD_WALL | WILD_ENTR,
/* 3M*/ WILD_TREE | WILD_HUT | WILD_WALL | WILD_ENTR,
/* 2M*/ WILD_TREE | WILD_HUT | WILD_WALL | WILD_ENTR
};
#endif

static wildObjGfx_t wild_treeGfxOff[] = {
	{{{0, 0, 43, 26, 0, 31}, {0, 0, 43, 26, 32, 31}, {0, 0, 43, 26, 52, 31}}},
	{{{20, 0, 55, 43, 0, 21}, {0, 0, 55, 43, 32, 21}, {0, 0, 55, 43, 52, 21}}},
	{{{60, 0, 111, 86, 0, 0}, {0, 0, 111, 86, 0, 0}, {0, 0, 51, 86, 60, 0}}}
};

static wildObjGfx_t wild_hutGfxOff[] = {
	{{{4, 0, 34, 26, 0, 33}, {0, 0, 34, 26, 32, 33}, {0, 0, 34, 26, 75, 33}}},
	{{{4, 0, 40, 42, 0, 21}, {0, 0, 40, 42, 32, 21}, {0, 0, 36, 42, 75, 21}}},
	{{{60, 0, 85, 73, 0, 10}, {0, 0, 85, 73, 0, 10}, {0, 0, 25, 73, 86, 10}}}
};

static wildObjGfx_t wild_cityWallGfxOff[] = {
	{{{13, 0, 35, 25, 22, 28}, {13, 0, 35, 25, 45, 28}, {13, 0, 35, 25, 68, 28}}},
	{{{5, 0, 42, 37, 0, 22}, {5, 0, 42, 37, 37, 22}, {5, 0, 42, 37, 74, 22}}},
	{{{44, 0, 69, 64, 0, 8}, {9, 0, 69, 64, 25, 8}, {9, 0, 34, 64, 86, 8}}}
};

static wildObjGfx_t wild_cityEntrGfxOff[] = {
	{{{13, 0, 35, 26, 22, 28}, {13, 0, 35, 26, 45, 28}, {13, 0, 35, 26, 68, 28}}},
	{{{5, 0, 42, 39, 0, 22}, {5, 0, 42, 39, 37, 22}, {5, 0, 42, 39, 74, 22}}},
	{{{44, 0, 69, 76, 0, 8}, {9, 0, 69, 76, 25, 8}, {9, 0, 34, 76, 86, 8}}}
};

static wildGfxOff_t wild_wallRtSideGfxOff[] = {
	{3, 0, 13, 87, 67, 0},
	{14, 0, 27, 87, 78, 0},
	{28, 0, 47, 87, 92, 0}
};

static wildGfxOff_t wild_entrRtSideGfxOff[] = {
	{7, 0, 23, 48, 63, 17},
	{3, 0, 27, 76, 69, 1},
	{5, 0, 39, 87, 77, 0}
};

static wildGfxOff_t wild_wallLtSideGfxOff[] = {
	{34, 0, 44, 87, 34, 0},
	{20, 0, 33, 86, 20, 0},
	{0, 0, 19, 87, 0, 0}
};

static wildGfxOff_t wild_entrLtSideGfxOff[] = {
	{2, 0, 16, 48, 34, 17},
	{4, 0, 26, 76, 20, 1},
	{0, 0, 34, 87, 0, 0}
};

static wildGfxOff_t wild_wallRtCrnrGfxOff[] = {
	{3, 0, 47, 37, 67, 21},
	{6, 0, 39, 64, 78, 6}
};

static wildGfxOff_t wild_wallLtCrnrGfxOff[] = {
	{0, 0, 44, 37, 0, 21},
	{0, 0, 33, 64, 0, 6}
};

/* graphics for a city wall at the edge */
static wildObjGfx_t wild_cityWallCrGfxOff[] = {
	{{
		{13, 0, 35, 25, 0, 28}, 
		{0, 0, 0, 0, 0, 0}, 
		{13, 0, 35, 25, 89, 28}
	}}
};

/* graphics for a city entrance at the edge */
static wildObjGfx_t wild_cityEntrCrGfxOff[] = {
	{{
		{13, 0, 35, 26, 0, 28}, 
		{0, 0, 0, 0, 0, 0}, 
		{13, 0, 35, 26, 89, 28}
	}}
};

/****************************************/
/*					*/
/* Local functions			*/
/*					*/
/****************************************/

static void wild_mirrorGfx(btwoGfx_t * fromStruct, btwoGfx_t * toStruct)
{
	uint16_t	fromIndex;
	uint16_t	toIndex;
	int		width;
	int		height;
	int		twidth;

	fromIndex = fromStruct->offset;
	toIndex = toStruct->offset;

	width = fromStruct->width << 2;
	twidth = width - 1;
	toIndex += twidth;
	height = fromStruct->height;

	while (height > 0) {
		twidth = width;
		while (twidth > 0) {
			wpic->buf[toIndex] = ((wpic->buf[fromIndex] & 0xf0) >> 4) | ((wpic->buf[fromIndex] & 0x0f) << 4);
			toIndex--;
			fromIndex++;
			twidth--;
		}
		toIndex += (width << 1);
		height--;
	}
}

static void wild_gfxInit(void)
{
	uint16_t	offset;
	int		counter;

	counter = 0;
	offset = 0;

	do {
		wild_treeGfx[counter].offset = offset;
		wild_treeGfx[counter].width = 
			(wild_treeGfxOff[counter].objs[1].right + 8) >> 3;
		wild_treeGfx[counter].height = 
			wild_treeGfxOff[counter].objs[1].top + 1;
		offset += (wild_treeGfx[counter].width * wild_treeGfx[counter].height) << 2;
		counter++;
	} while (counter < 3);

	counter = 0;
	do {
		wild_hutGfx[counter].offset = offset;
		wild_hutGfx[counter].width = (wild_hutGfxOff[counter].objs[1].right + 8) >> 3;
		wild_hutGfx[counter].height = wild_hutGfxOff[counter].objs[1].top + 1;
		offset += (wild_hutGfx[counter].width * wild_hutGfx[counter].height) << 2;
		counter++;
	} while (counter < 3);

	counter = 0;
	do {
		wild_cityWallGfx[counter].offset = offset;
		wild_cityWallGfx[counter].width = (wild_cityWallGfxOff[counter].objs[1].right + 8) >> 3;
		wild_cityWallGfx[counter].height = wild_cityWallGfxOff[counter].objs[1].top + 1;
		offset += (wild_cityWallGfx[counter].width * wild_cityWallGfx[counter].height) << 2;
		counter++;
	} while (counter < 3);

	counter = 0;
	do {
		wild_cityEntrGfx[counter].offset = offset;
		wild_cityEntrGfx[counter].width = (wild_cityEntrGfxOff[counter].objs[1].right + 8) >> 3;
		wild_cityEntrGfx[counter].height = wild_cityEntrGfxOff[counter].objs[1].top + 1;
		offset += (wild_cityEntrGfx[counter].width * wild_cityEntrGfx[counter].height) << 2;
		counter++;
	} while (counter < 3);

	counter = 0;
	do {
		wild_wallLtSideGfx[counter].offset = offset;
		wild_wallLtSideGfx[counter].width = 6;
		wild_wallLtSideGfx[counter].height = 88;
		counter++;
	} while (counter < 3);

	counter = 0;
	offset += 0x840;
	do {
		wild_entrLtSideGfx[counter].offset = offset;
		wild_entrLtSideGfx[counter].width = (wild_entrLtSideGfxOff[counter].right + 8) >> 3;
		wild_entrLtSideGfx[counter].height = wild_entrLtSideGfxOff[counter].top + 1;
		offset += (wild_entrLtSideGfx[counter].width * wild_entrLtSideGfx[counter].height) << 2;
		counter++;
	} while (counter < 3);

	counter = 0;
	do {
		wild_wallLtCrnrGfx[counter].offset = offset;
		wild_wallLtCrnrGfx[counter].width = (wild_wallLtCrnrGfxOff[counter].right + 8) >> 3;
		wild_wallLtCrnrGfx[counter].height = wild_wallLtCrnrGfxOff[counter].top + 1;
		offset += (wild_wallLtCrnrGfx[counter].width * wild_wallLtCrnrGfx[counter].height) << 2;
		counter++;
	} while (counter < 2);

	counter = 0;
	do {
		wild_wallRtSideGfx[counter].offset = offset;
		wild_wallRtSideGfx[counter].width = 6;
		wild_wallRtSideGfx[counter].height = 88;
		counter++;
	} while (counter < 3);

	offset += 0x840;
	counter = 0;
	do {
		wild_entrRtSideGfx[counter].offset = offset;
		wild_entrRtSideGfx[counter].width = (wild_entrRtSideGfxOff[counter].right + 8) >> 3;
		wild_entrRtSideGfx[counter].height = wild_entrRtSideGfxOff[counter].top + 1;
		offset += (wild_entrRtSideGfx[counter].width * wild_entrRtSideGfx[counter].height) << 2;
		counter++;
	} while (counter < 3);

	counter = 0;
	do {
		wild_wallRtCrnrGfx[counter].offset = offset;
		wild_wallRtCrnrGfx[counter].width = (wild_wallRtCrnrGfxOff[counter].right + 8) >> 3;
		wild_wallRtCrnrGfx[counter].height = wild_wallRtCrnrGfxOff[counter].top + 1;
		offset += (wild_wallRtCrnrGfx[counter].width * wild_wallRtCrnrGfx[counter].height) << 2;
		counter++;
	} while (counter < 2);

	wild_mirrorGfx(wild_wallLtSideGfx, wild_wallRtSideGfx);

	for (counter = 0; counter < 3; counter++) {
		wild_mirrorGfx(
			&wild_entrLtSideGfx[counter], 
			&wild_entrRtSideGfx[counter]
		);
	}

	for (counter = 0; counter < 2; counter++) {
		wild_mirrorGfx(
			&wild_wallLtCrnrGfx[counter], 
			&wild_wallRtCrnrGfx[counter]
		);
	}
}



static void getWildGfx(bt_view_t *view)
{
	uint8_t		i;

	i = 0;
	while (dunTreeHut[i].depth) {
		outputWildFacet(view, &dunTreeHut[i], WILD_HUT);
		outputWildFacet(view, &dunTreeHut[i], WILD_TREE);
		i++;
	}

	i = 0;
	while (dunWall[i].depth) {
		outputWildFacet(view, &dunWall[i], WILD_WALL);
		i++;
	}

	i = 0;
	while (dunEntr[i].depth) {
		debug("Entrance: %d\n", i);
		outputWildFacet(view, &dunEntr[i], WILD_ENTR);
		i++;
	}
}

static void getWildData(dunfacet_t *df, uint8_t facet, 
			btwoGfx_t **wild, wildGfxOff_t **sz)
{
	if (facet == WILD_WALL) {
		switch (df->facet) {
		case WILD_FRONT:
			*wild	= &wild_cityWallGfx[df->faceMap];
			*sz	= &wild_cityWallGfxOff[df->faceMap].objs[df->rightFlag];
			break;
		case WILD_EDGE:
			debug("df->faceMap: %d, df->rightFlag: %d\n", df->faceMap, df->rightFlag);
			*wild	= &wild_cityWallGfx[df->faceMap];
/*			*sz	= &wild_cityWallCrGfxOff[df->faceMap].objs[df->rightFlag];*/
			*sz	= &wild_cityWallCrGfxOff[0].objs[df->rightFlag];
			break;
		case WILD_SIDE:
			if (!df->rightFlag) {
				*wild = &wild_wallLtSideGfx[df->faceMap];
				*sz   = &wild_wallLtSideGfxOff[df->faceMap];
			} else {
				*wild = &wild_wallRtSideGfx[df->faceMap];
				*sz   = &wild_wallRtSideGfxOff[df->faceMap];
			}
			break;
		case WILD_CORNER:
			if (!df->rightFlag) {
				*wild = &wild_wallLtCrnrGfx[df->faceMap];
				*sz   = &wild_wallLtCrnrGfxOff[df->faceMap];
			} else {
				*wild = &wild_wallRtCrnrGfx[df->faceMap];
				*sz   = &wild_wallRtCrnrGfxOff[df->faceMap];
			}
			break;
		}
	} else {
		switch (df->facet) {
		case WILD_FRONT:
			*wild	= &wild_cityEntrGfx[df->faceMap];
			*sz	= &wild_cityEntrGfxOff[df->faceMap].objs[df->rightFlag];
			break;
		case WILD_EDGE:
			*wild	= &wild_cityEntrGfx[df->faceMap];
			*sz	= &wild_cityEntrCrGfxOff[df->faceMap].objs[df->rightFlag];
			break;
		case WILD_SIDE:
			if (!df->rightFlag) {
				*wild = &wild_entrLtSideGfx[df->faceMap];
				*sz   = &wild_entrLtSideGfxOff[df->faceMap];
			} else {
				*wild = &wild_entrRtSideGfx[df->faceMap];
				*sz   = &wild_entrRtSideGfxOff[df->faceMap];
			}
			break;
		}
	}
}

static uint8_t *sideString[] = { "left", "middle", "right" };
static uint8_t *facetString[] = { 
	"",
	"tree", "hut", "wall", "entr", 
	"wall", "wall", "entr", "entr",
	"wall", "wall",
	"wall", "entr"
};

static void outputWildFacet(bt_view_t *oview, dunfacet_t *df, uint8_t facet)
{
	bta_cell_t	*img;

	btwoGfx_t	*wild;
	wildGfxOff_t	*sz;
	wildQmap_t	*qmap;
	int16_t		_src_skip;
	uint16_t	x, y;
	uint16_t	offset;
	uint16_t	width, height;
	view_t		view;

	switch (facet) {
	case WILD_TREE:
		wild	= &wild_treeGfx[df->faceMap];
		sz 	= &wild_treeGfxOff[df->faceMap].objs[df->rightFlag];
		break;
	case WILD_HUT:
		wild	= &wild_hutGfx[df->faceMap];
		sz 	= &wild_hutGfxOff[df->faceMap].objs[df->rightFlag];
		break;
	case WILD_WALL:
		getWildData(df, facet, &wild, &sz);
		debug("wild = %p, sz = %p\n", wild, sz);
		break;
	case WILD_ENTR:
		getWildData(df, facet, &wild, &sz);
		debug("wild = %p, sz = %p\n", wild, sz);
		break;
	}

	view.x = sz->x;
	view.y = sz->y;
	view.rflag = 0;
	view.left = sz->left;
	view.right = sz->right;
	debug("sz->right = %d\n", sz->right);
	view.top = sz->top;
	view.bottom = sz->bottom;
	view.width = wild->width;
	view.offset = wild->offset;

	debug("wpics/%d-%s/%s%s.png\n", df->depth, df->name, facetString[facet], df->extra);
	img = getImage(&view, wpic);

	xmkdir(mkImagePath("wpics/%d-%s", df->depth, df->name));	

	bta_toPNG(img,
		mkImagePath("wpics/%d-%s/%s%s.png",
				df->depth,
				df->name,
				facetString[facet],
				df->extra)
		);
	bt_view_new_facet(oview,
			bts_sprintf("%d-%s", df->depth, df->name),
			bts_sprintf("%s%s", facetString[facet], df->extra),
			view.x, view.y, img->width, img->height);
	bta_cell_free(img);
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
		b->buf[i] = 0x22;

	bg = bta_cell_new(0, 0, 56, 88, 0, b);
	bg = bta_cell_convert(bg);

	bta_toPNG(bg, mkImagePath("wpics/wpic.bg"));

	bta_cell_free(bg);
}

/****************************************/
/*					*/
/* Public functions			*/
/*					*/
/****************************************/

void outputWildpics(uint8_t indent)
{
	bt_view_t	*view;

	xmkdir(mkImagePath("wpics"));

	outputBackground();

	wpic = dehufFile(mkBardTwoPath("WPICS"), 0x6b88);
	wpic = bts_resize(wpic, 0xe034);

	wild_gfxInit();

	view = bt_view_new();

	getWildGfx(view);

	bt_view_to_json(view, mkJsonPath("wildview.json"));
	bt_view_free(view);

	bts_free(wpic);
}
