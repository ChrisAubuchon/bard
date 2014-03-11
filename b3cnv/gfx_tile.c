#include <b3lib.h>
#include <gfx.h>
#include <cnv_gfx.h>
#include <gfx_tile.h>

#define DEBUG
#include <debug.h>

#define NEWVERSION 1

#define DUN_FRONT	0x01
#define DUN_LTSIDE	0x02
#define DUN_RTSIDE	0x04

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/

static void		buildView(bt_view_t *view, cityTile_t *tiles,
				uint32_t tagIndex);
static void		makeImages(cityTile_t *tile, uint32_t tagIndex,
				uint8_t width, uint32_t imageCount);
static void		copy_topoElement(b3tile_t *tile);
static void		outputBackground(uint32_t tagIndex);
static void		initBuffers(void);
static uint8_t		getHeight(uint8_t h, uint8_t a_e);


/****************************************/
/*					*/
/* Static variables			*/
/*					*/
/****************************************/

static bta_color_t b3palette[] = {
        {   0,   0,   0, 255 }, /*0*/
        {   0,   0, 170, 255 }, /*1*/
        {   0, 170,   0, 255 }, /*2*/
        {   0, 170, 170, 255 }, /*3*/
        { 170,   0,   0, 255 }, /*4*/
        { 170,   0, 170, 0   }, /*5*/
        { 170,  85,   0, 255 }, /*6*/
        { 170, 170, 170, 255 }, /*7*/
        {  85,  85,  85, 255 }, /*8*/
        {  85,  85, 255, 255 }, /*9*/
        {  85, 255,  85, 255 }, /*a*/
        {  85, 255, 255, 255 }, /*b*/
        { 255,  85,  85, 255 }, /*c*/
        { 255,  85, 255, 255 }, /*d*/
        { 255, 255,  85, 255 }, /*e*/
        { 255, 255, 255, 255 }  /*f*/
};

static cityTile_t cityTiles[] = {
	{ 4, "FL", "face",	30	},	
	{ 4, "L" , "face",	31	},	
	{ 4, "M" , "face",	32	},	
	{ 4, "R" , "face",	33	},	
	{ 4, "FR", "face",	34	},	
	{ 3, "L" , "face",	45	},
	{ 3, "L" , "side",	42	},
	{ 3, "M" , "face",	46	},
	{ 3, "R" , "face",	47	},
	{ 3, "R" , "side",	43	},
	{ 2, "L" , "side",	54	},
	{ 2, "L" , "face",	56	},
	{ 2, "M" , "face",	57	},
	{ 2, "R" , "face",	58	},
	{ 2, "R" , "side",	55	},
	{ 1, "L" , "side",	59	},
	{ 1, "R" , "side",	60	},
	{ 0, ""  , "",		0	}
};

static cityTile_t dunTiles[] = {
	{ 5, "L" , "face",	15	},	
	{ 5, "M" , "face",	16	},	
	{ 5, "R" , "face",	17	},	
	{ 4, "L" , "face",	31	},	
	{ 4, "L" , "side",	27	},	
	{ 4, "M" , "face",	32	},	
	{ 4, "R" , "face",	33	},	
	{ 4, "R" , "side",	28	},	
	{ 3, "L" , "face",	45	},	
	{ 3, "L" , "side",	42	},	
	{ 3, "M" , "face",	46	},	
	{ 3, "R" , "face",	47	},	
	{ 3, "R" , "side",	43	},	
	{ 2, "L" , "face",	56	},	
	{ 2, "L" , "side",	54	},	
	{ 2, "M" , "face",	57	},	
	{ 2, "R" , "face",	58	},	
	{ 2, "R" , "side",	55	},	
	{ 1, "L" , "side",	59	},	
	{ 1, "R" , "side",	60	},	
	{ 0, ""  , "",		0	}
};

static uint8_t *gfx_fnames[] = { "skara.grp", "wildwal.grp", "gdung.grp" };
static uint8_t *gfx_tags[] = { "skara", "wild", "gdung" };

static quadXY_t quadXYmap[] = {
	{12, 42}, {15, 42}, {18, 42}, {21, 42},
	{24, 42}, {27, 42}, {30, 42}, {5, 38},
	{12, 38}, {26, 38}, {29, 38}, {31, 38},
	{39, 38}, {-2, 38}, {5, 38}, {17, 44},
	{24, 44}, {31, 44}, {33, 38}, {40, 38},
	{13, 33}, {21, 33}, {31, 33}, {13, 55},
	{21, 55}, {31, 55}, {-1, 38}, {23, 38},
	{31, 38}, {42, 38}, {2, 42}, {12, 42},
	{23, 42}, {34, 42}, {45, 42}, {4, 21},
	{20, 21}, {34, 21}, {4, 63}, {20, 63},
	{34, 63}, {3, 35}, {19, 35}, {33, 35},
	{48, 35}, {0, 35}, {18, 35}, {37, 35},
	{0, 8}, {17, 4}, {44, 8}, {0, 77},
	{17, 77}, {44, 77}, {12, 22}, {37, 22},
	{-21, 22}, {11, 22}, {43, 22}, {0, 0}, {44, 0}
};

static uint8_t b44278[] = {
/* 0*/ 3, 3, 3, 3, 3, 3, 3, 1, 1, 1,
/*10*/ 1, 1, 1, 6, 6, 6, 6, 6, 6, 6,
/*20*/ 10, 10, 10, 10, 10, 10, 2, 2, 2, 2,
/*30*/ 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
/*40*/ 10, 3, 3, 3, 3, 18, 18, 18, 10, 10,
/*50*/ 10, 10, 10, 10, 6, 6, 32, 32, 32, 11,
/*60*/ 11, 0
};

static uint8_t b442f4[] = {
	2, 2, 2, 2, 2, 2, 2, 0,
	0, 0, 0, 0, 0, 2, 2, 2,
	2, 2, 2, 2, 0, 0, 0, 2,
	2, 2, 0, 0, 0, 0, 2, 2,
	2, 2, 2, 0, 0, 0, 2, 2,
	2, 0, 0, 0, 0, 2, 2, 2,
	0, 0, 0, 2, 2, 2, 0, 0,
	2, 2, 2, 0, 0, 0
};

static uint8_t topo_rightFlag[] = {
/*  0*/	0, 0, 0, 0,
/*  4*/ 0, 0, 0, 0,
/*  8*/ 0, 0, 1, 1,
/* 12*/ 1, 0, 0, 0,
/* 16*/ 0, 0, 0, 0,
/* 20*/ 0, 0, 1, 0,
/* 24*/ 0, 1, 0, 0,
/* 28*/ 1, 1, 0, 0,
/* 32*/ 0, 0, 0, 0,
/* 36*/ 0, 1, 0, 0,
/* 40*/ 1, 0, 0, 1,
/* 44*/ 1, 0, 0, 0,
/* 48*/ 0, 0, 1, 0,
/* 52*/ 0, 1, 0, 1,
/* 56*/ 0, 0, 0, 0,
/* 60*/ 1, 0
};

static uint8_t b442b6[] = {
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 12, 12, 12,
	12, 12, 12, 12, 64, 64, 64, 64,
	64, 64, 14, 14, 14, 14, 20, 20,
	20, 20, 20, 64, 64, 64, 64, 64,
	64, 20, 20, 20, 20, 36, 36, 36,
	64, 64, 64, 64, 64, 64, 36, 36,
	64, 64, 64, 64, 64, 0
};

static uint8_t b2bf34[0x100];
static uint8_t b2c034[0x100];
static uint8_t b2c134[0x100];
static uint8_t b2c234[0x100];

static btstring_t *dundata;

/****************************************/
/*					*/
/* Local functions			*/
/*					*/
/****************************************/

static uint8_t getHeight(uint8_t h, uint8_t a_e)
{
	int16_t o = 0;
	uint32_t i;
	uint8_t rval = 0;

	for (i = 0; i < h; i++) {
		if ((o -= a_e) < 0) {
			o += 64;
			rval++;
		}
	}

	return rval;
}

static void initBuffers(void)
{
	uint32_t i;

	for (i = 0; i < 256; i++) {
		b2bf34[i] = i & 0xf0;
		b2c234[i] = i & 0x0f;
		b2c134[i] = b2c234[i] << 4;
		b2c034[i] = b2bf34[i] >> 4;
	}
}

static void copy_topoElement(b3tile_t *tile)
{
	uint8_t		y		= 0;
	uint16_t	offset		= 0;
	int16_t		src		= 0;
	uint16_t	outOffset;
	uint32_t	i;

	uint8_t		inCh, outCh;
	uint16_t	base;
	uint8_t		width;
	int8_t		count;

	for (i = 0; i < tile->height; i++) {
		if ((src -= tile->scale) < 0) {
			src += 64;
			outOffset = (tile->width + 1) * y;
			count = 0;

			width = tile->width;

			if (tile->rightFlag)
				base = tile->base + offset + (tile->srcSkip-1);
			else
				base = tile->base + offset;

		      loc_loopStart:
			if (tile->rightFlag)
				inCh = dundata->buf[base--];
			else
				inCh = dundata->buf[base++];

			if ((count -= tile->scale) < 0) {
				count += 64;
				outCh = b2bf34[inCh];

			      loc_loop_1:
				if ((count -= tile->scale) >= 0) {
				      loc_loop_2:
					if (tile->rightFlag)
						inCh = dundata->buf[base--];
					else
						inCh = dundata->buf[base++];
					if ((count -= tile->scale) >= 0) {
						goto loc_loop_1;
					} else {
						count += 64;
						outCh |= b2c034[inCh];
						tile->cell->gfx->buf[outOffset++] = outCh;
						if (width) {
							width--;
							goto loc_loop_3;
						} else {
							goto loc_out;
						}
					}
				} else {
					count += 64;
					outCh |= b2c234[inCh];
					tile->cell->gfx->buf[outOffset++] = outCh;
					if (width) {
						width--;
						goto loc_loopStart;
					} else {
						goto loc_out;
					}
				}
			}

		      loc_loop_3:
			if ((count -= tile->scale) >= 0)
				goto loc_loopStart;

			count += 64;
			outCh = b2c134[inCh];
			goto loc_loop_2;

		      loc_out:
			y++;
		}
	offset += tile->srcSkip;
	}
}

/*
 * getBackground()
 */
static void outputBackground(uint32_t tagIndex)
{
	bta_cell_t	*bg;
	btstring_t	*b;
	uint32_t	i;
	uint16_t	index;

	b = bts_new(4928);

	if (tagIndex < 2) {
		for (i = 0; i < 2576; i++)
			b->buf[i] = 0xbb;
		for (; i < 4928; i++) 
			b->buf[i] = 0x44;
	} else {
		index = str_read16le(&dundata->buf[0x10]) + 0x0d;
		for (i = 0; i < 0x9a0; i++)
			b->buf[i] = dundata->buf[index + i];

		index = str_read16le(&dundata->buf[0x12]) + 0x0d;
		for (i = 0; i < 0x9a0; i++)
			b->buf[i + 0x9a0] = dundata->buf[index + i];
	}

	bg = bta_cell_new(0, 0, 56, 88, 0, b);
	bg = bta_cell_convert(bg);

	bta_toPNG(bg, mkImagePath("%s/bg-day.png", gfx_tags[tagIndex]));
	bta_cell_free(bg);

	b = bts_new(4928);

	if (tagIndex < 2) {
		for (i = 0; i < 2576; i++)
			b->buf[i] = 0x00;
		for (; i < 4928; i++)
			b->buf[i] = 0x44;
	} else {
		index = str_read16le(&dundata->buf[0x10]) + 0x0d;
		for (i = 0; i < 0x9a0; i++)
			b->buf[i] = dundata->buf[index + i];

		index = str_read16le(&dundata->buf[0x12]) + 0x0d;
		for (i = 0; i < 0x9a0; i++)
			b->buf[i + 0x9a0] = dundata->buf[index + i];
	}

	bg = bta_cell_new(0, 0, 56, 88, 0, b);
	bg = bta_cell_convert(bg);

	bta_toPNG(bg, mkImagePath("%s/bg-night.png", gfx_tags[tagIndex]));
	bta_cell_free(bg);
}

static void makeImages(cityTile_t *tile, uint32_t tagIndex, uint8_t width,
		uint32_t imageCount)
{
	uint32_t	i;
	b3tile_t	*t;
	uint16_t	offset, base;
	uint8_t		height;

	for (i = 0; i < imageCount; i++) {
		t = xzalloc(sizeof(b3tile_t));

		t->cell = bta_cell_new(0, 0, 0, 0, 0, NULL);
		base = (i * 4) + b442f4[tile->index];

		offset = str_read16le(&dundata->buf[base]);
		offset += 9;

		t->width	= width;
		t->srcSkip	= dundata->buf[offset++];
		t->height	= dundata->buf[offset++];
		if (t->height == 0) {
			bta_cell_free(t->cell);
			free(t);
			continue;
		}

		height = getHeight(t->height, b442b6[tile->index]);
		offset += 2;

		t->rightFlag	= topo_rightFlag[tile->index];
		t->scale	= b442b6[tile->index];

		t->cell->width	= width + 1;
		t->cell->height	= height;
		t->cell->gfx	= bts_new(t->cell->width * t->cell->height);
		t->base		= offset;

		copy_topoElement(t);

		t->cell	= bta_cell_4bitTo8bit(t->cell);
		t->cell	= bta_cell_scale(t->cell);
		t->cell	= bta_cell_toRGBA(t->cell, b3palette);

		bta_toPNG(t->cell,
			mkImagePath("%s/%d%s/%d-%s.png",
				gfx_tags[tagIndex],
				tile->depth,
				tile->quad,
				i,
				tile->facet
				)
			);

		bta_cell_free(t->cell);
		free(t);
	}

}

/*
 * buildView()
 */
static void buildView(bt_view_t *view, cityTile_t *tiles, uint32_t tagIndex)
{
	uint32_t	i;
	int32_t		x, y;
	uint8_t		imageCount;
	uint8_t		width;

	imageCount = str_read16le(dundata->buf) / 4;

	i = 0;
	while (tiles[i].depth) {
		x = quadXYmap[tiles[i].index].column;
		y = quadXYmap[tiles[i].index].row;

		if (x < 0) {
			if ((x + b44278[tiles[i].index]) < 0)
				width = 0;
			else
				width = x + b44278[tiles[i].index];
			x = 0;
		} else {
			if ((x + b44278[tiles[i].index]) > 56)
				width = 55 - x;
			else
				width = b44278[tiles[i].index];
		}

		xmkdir(mkImagePath("%s/%d%s",
			gfx_tags[tagIndex],
			tiles[i].depth,
			tiles[i].quad
			));

		makeImages(&tiles[i], tagIndex, width, imageCount);

		bt_view_new_facet(
			view,
			bts_sprintf("%d%s", tiles[i].depth, tiles[i].quad),
			bts_strcpy(tiles[i].facet),
			x << 2, y << 1, 0, 0
			);

		i++;
	}
}

/****************************************/
/*					*/
/* Public functions			*/
/*					*/
/****************************************/



void outputTilepics(uint8_t indent)
{
	uint32_t	i;
	bt_view_t	*view;


	initBuffers();

	for (i = 0; i < 3; i++) {
		view = bt_view_new();

		xmkdir(mkImagePath("%s", gfx_tags[i]));

		dundata = fp_readFile(mkBardThreePath("%s", gfx_fnames[i]));

		outputBackground(i);

		if (i < 2)
			buildView(view, cityTiles, i);
		else
			buildView(view, dunTiles, i);

		bt_view_to_json(view, mkJsonPath("%sview.json", gfx_tags[i]));

		bt_view_free(view);
		bts_free(dundata);
	}

}
