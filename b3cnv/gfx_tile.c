#include <b3lib.h>
#include <gfx.h>
#include <cnv_gfx.h>

/*#define DEBUG*/
#include <debug.h>

#define DUN_FRONT	0x01
#define DUN_LTSIDE	0x02
#define DUN_RTSIDE	0x04

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/

static void copy_topoElement(uint16_t base, uint8_t width, uint8_t srcSkip, uint8_t height, uint8_t a_e, uint8_t rightFlag,
			     btstring_t * data);
static btstring_t *getBackground(uint8_t dunFlag);
static void initBuffers(void);
static uint8_t getHeight(uint8_t h, uint8_t a_e);

static void tile2png(uint8_t dindex, uint32_t findex, uint8_t dunflag);

/****************************************/
/*					*/
/* Static variables			*/
/*					*/
/****************************************/

static uint8_t *gfx_fnames[] = { "skara.grp", "wildwal.grp", "gdung.grp" };
static uint8_t *gfx_tags[] = { "tile_skara", "tile_wild", "tile_gdung" };
static uint8_t wild_images[] = {
	30, 42, 43, 45, 54, 55, 56, 57, 58, 59, 60
};

static uint8_t wild_dims[] = {
	30, 31, 32, 33, 34, 42, 43, 45, 46, 47, 54, 55, 56, 57, 58, 59, 60
};

static uint8_t *wild_face_desc[] = {
	"_4_face",
	"_3_leftside",
	"_3_rightside",
	"_3_face",
	"_2_leftside",
	"_2_rightside",
	"_2_leftface",
	"_2_face",
	"_2_rightface",
	"_1_leftside",
	"_1_rightside"
};

static uint8_t wild_facets[] = {
	DUN_FRONT, 
	DUN_LTSIDE, DUN_RTSIDE, DUN_FRONT, 
	DUN_LTSIDE, DUN_RTSIDE, DUN_FRONT, DUN_FRONT, DUN_FRONT,
	DUN_LTSIDE, DUN_RTSIDE
};

static uint8_t dun_images[] = {
	15, 27, 28, 31, 42, 43, 45, 54, 55, 56, 57, 58, 59, 60
};

static uint8_t *dun_face_desc[] = {
	"_4_face",
	"_4_leftside",
	"_4_rightside",
	"_3_face",
	"_3_leftside",
	"_3_rightside",
	"_2_face",
	"_2_leftside",
	"_2_rightside",
	"_1_leftface",
	"_1_face",
	"_1_rightface",
	"_1_leftside",
	"_1_rightside"
};

static quadXY_t quadXYmap[] = {
/*0*/ {12, 42}, {15, 42}, {18, 42}, {21, 42}, {24, 42}, {27, 42}, {30, 42}, {5, 38},
/*8*/ {12, 38}, {26, 38}, {29, 38}, {31, 38}, {39, 38}, {-2, 38}, {5, 38}, {17, 44},
/*16*/ {24, 44}, {31, 44}, {33, 38}, {40, 38}, {13, 33}, {21, 33}, {31, 33}, {13, 55},
/*24*/ {21, 55}, {31, 55}, {-1, 38}, {23, 38}, {31, 38}, {42, 38}, {2, 42}, {12, 42},
/*32*/ {23, 42}, {34, 42}, {45, 42}, {4, 21}, {20, 21}, {34, 21}, {4, 63}, {20, 63},
/*40*/ {34, 63}, {3, 35}, {19, 35}, {33, 35}, {48, 35}, {0, 35}, {18, 35}, {37, 35},
/*48*/ {0, 8}, {17, 4}, {44, 8}, {0, 77}, {17, 77}, {44, 77}, {12, 22}, {37, 22},
/*56*/ {-21, 22}, {11, 22}, {43, 22}, {0, 0}, {44, 0}
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
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 0, 0, 1, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	1, 0, 0, 1, 1, 0, 0, 0, 0, 0,
	1, 0, 0, 1, 0, 1, 0, 0, 0, 0,
	1, 0
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
static uint8_t b2c334[0x100];

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
		b2c334[i] = ((b2c234[i] == 0x05) ? 0x0f : 0) | ((b2bf34[i] == 0x50) ? 0xf0 : 0);
		b2c034[i] = b2bf34[i] >> 4;
	}
}

static void copy_topoElement(uint16_t base, uint8_t width, uint8_t srcSkip, uint8_t height, uint8_t a_e, uint8_t rightFlag,
			     btstring_t * data)
{
	uint32_t i;
	int16_t src = 0;
	uint8_t y = 0;
	uint16_t outOffset;
	uint16_t offset = 0;

	for (i = 0; i < height; i++) {
		if ((src -= a_e) < 0) {
			src += 64;
			outOffset = (width + 1) * y;

			{
				int8_t count = 0;
				uint8_t inCh, outCh, tmp;
				uint16_t b;
				uint8_t w;

				w = width;

				if (rightFlag)
					b = base + offset + (srcSkip - 1);
				else
					b = base + offset;

			      loc_loopStart:
				if (rightFlag)
					inCh = dundata->buf[b--];
				else
					inCh = dundata->buf[b++];

				if ((count -= a_e) < 0) {
					count += 64;
					outCh = b2bf34[inCh];

				      loc_loop_1:
					if ((count -= a_e) >= 0) {
					      loc_loop_2:
						if (rightFlag)
							inCh = dundata->buf[b--];
						else
							inCh = dundata->buf[b++];
						if ((count -= a_e) >= 0) {
							goto loc_loop_1;
						} else {
							count += 64;
							outCh |= b2c034[inCh];
							tmp = outCh;
							outCh &= b2c334[tmp];
							outCh ^= tmp;
							data->buf[outOffset++] = outCh;
							if (w) {
								w--;
								goto loc_loop_3;
							} else {
								goto loc_out;
							}
						}
					} else {
						count += 64;
						outCh |= b2c234[inCh];
						inCh = outCh;
						outCh &= b2c334[inCh];
						outCh ^= inCh;
						data->buf[outOffset++] = outCh;
						if (w) {
							w--;
							goto loc_loopStart;
						} else {
							goto loc_out;
						}
					}
				}

			      loc_loop_3:
				if ((count -= a_e) >= 0)
					goto loc_loopStart;

				count += 64;
				outCh = b2c134[inCh];
				goto loc_loop_2;
			}

		      loc_out:
			y++;
		}

		offset += srcSkip;
	}
}

static void tile2png(uint8_t dindex, uint32_t findex, uint8_t dunflag)
{
	bta_cell_t *rval;
	uint8_t facet;
	uint8_t nimgs;
	uint8_t i;
	uint16_t offset, baseOffset;
	uint8_t srcSkip;
	uint8_t h, height, width;
	int8_t x, y;

	nimgs = str_read16le(dundata->buf) / 4;

	facet = (dunflag) ? dun_images[findex] : wild_images[findex];

	x = quadXYmap[facet].column;
	y = quadXYmap[facet].row;

	if (x < 0) {
		if ((x + b44278[facet]) < 0)
			width = 0;
		else
			width = x + b44278[facet];
	} else {
		if ((x + b44278[facet]) > 56)
			width = 55 - x;
		else
			width = b44278[facet];
	}

	debug("facet: %d, x = %d, width = %d\n", facet, quadXYmap[facet].column, width);
	height = 0;

	for (i = 0; i < nimgs; i++) {
		rval = bta_cell_new(0, 0, 0, 0, 0, NULL);
		baseOffset = (i * 4) + b442f4[facet];

		offset = str_read16le(&dundata->buf[baseOffset]);
		offset += 9;

		srcSkip = dundata->buf[offset++];
		h = dundata->buf[offset++];
		if (h == 0) 
			continue;
		height = getHeight(h, b442b6[facet]);
		offset += 2;

		rval->width = width + 1;
		rval->height = height;
		rval->gfx = bts_new(rval->width * rval->height);
		copy_topoElement(offset, width, srcSkip, h, b442b6[facet], topo_rightFlag[facet], rval->gfx);
		rval = bta_cell_convert(rval);
		bta_toPNG(rval, bts_sprintf("%s_%d%s.png", gfx_tags[dindex], i, (dunflag) ? dun_face_desc[findex] : wild_face_desc[findex]));
		bta_cell_free(rval);
	}
}

static btstring_t *getBackground(uint8_t dunFlag)
{
	btstring_t *rval = NULL;
	uint32_t i, j;

	rval = bts_new(4928);

	if (dunFlag) {

		i = str_read16le(&dundata->buf[0x10]) + 0x0d;
		for (j = 0; j < 0x9a0; j++)
			rval->buf[j] = dundata->buf[i + j];

		i = str_read16le(&dundata->buf[0x12]) + 0x0d;
		for (j = 0; j < 0x9a0; j++)
			rval->buf[j + 0x9a0] = dundata->buf[i + j];

	} else {
		for (i = 0; i < 2576; i++)
			rval->buf[i] = 0x44;

		for (; i < 4928; i++)
			rval->buf[i] = 0xbb;
	}

	return rval;
}

/****************************************/
/*					*/
/* Public functions			*/
/*					*/
/****************************************/



void outputTilepics(uint8_t indent)
{
	huffile_t *huf;
	int i, j, k;
	bta_cell_t *cell;

	initBuffers();

	for (i = 0; i < 2; i++) {

		dundata = readFile(gfx_fnames[i]);

		for (j = 0; j < 11; j++) {
			tile2png(i, j, 0);
		}

		bts_free(dundata);
	}

	i = 2;

	dundata = readFile(gfx_fnames[i]);
	for (j = 0; j < 14; j++) {
		tile2png(i, j, 1);
	}

	bts_free(dundata);

}
