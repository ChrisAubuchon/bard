#include <b3lib.h>
#include <gfx.h>
#include <cnv_gfx.h>
#include <cnv_bigpic.h>
#include <dehuf.h>

/*#define DEBUG */
#include <debug.h>

/****************************************/
/*					*/
/* Local Macros				*/
/*					*/
/****************************************/
#define	PICBUF_SIZE	19712
#define NPIC_TOTAL	85

/****************************************/
/*					*/
/* Global variables			*/
/*					*/
/****************************************/

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/

/****************************************/
/*					*/
/* Static variables			*/
/*					*/
/****************************************/

static pic_t pics[] = {
/*   */ {0, 0}, {0, 1},
/*   */ {0, 2}, {0, 3},
/*   */ {0, 4}, {0, 5},
/*   */ {0, 6}, {0, 7},
/*   */ {0, 8}, {0, 9},
/*   */ {0, 10}, {0, 11},
/*   */ {0, 12}, {0, 13},
/*   */ {0, 14}, {0, 15},
/*   */ {0, 16}, {0, 17},
/*   */ {0, 18}, {0, 19},
/*   */ {0, 20}, {0, 21},
/*   */ {1, 8}, {1, 9},
/*   */ {0, 22}, {1, 10},
/*   */ {0, 23}, {1, 11},
/*   */ {0, 24}, {0, 25},
/*   */ {1, 12}, {0, 26},
/*   */ {0, 27}, {1, 14},
/*   */ {1, 15}, {1, 16},
/*   */ {1, 17}, {0, 28},
/*   */ {0, 29}, {1, 19},
/*   */ {1, 20}, {1, 21},
/*   */ {0, 30}, {1, 22},
/*   */ {1, 23}, {0, 31},
/*   */ {0, 32}, {0, 33},
/*   */ {0, 34}, {0, 35},
/*   */ {0, 36}, {0, 37},
/*   */ {0, 38}, {0, 39},
/*   */ {0, 40}, {0, 41},
/*   */ {0, 42}, {0, 43},
/*   */ {0, 44}, {1, 30},
/*   */ {0, 45}, {1, 32},
/*   */ {0, 46}, {1, 33},
/*   */ {1, 34}, {1, 35},
/*   */ {1, 36}, {1, 37},
/*   */ {0, 47}, {0, 48},
/*   */ {0, 49}, {1, 38},
/*   */ {1, 39}, {0, 50},
/*   */ {1, 40}, {1, 41},
/*   */ {1, 42}, {0, 51},
/*   */ {0, 52}, {1, 45},
/*   */ {1, 46}, {0, 53},
/*   */ {1, 47}, {1, 48},
/*   */ {1, 49}
};

/****************************************/
/*					*/
/* Local functions			*/
/*					*/
/****************************************/

void outputBigpic(uint8_t indent)
{
	FILE		*lowfp;
	FILE		*hifp;
	FILE		*curfp;

	uint8_t		*picBuf;

	int		i, j;

	btstring_t	*data;
	btstring_t	*bbuf;
	btstring_t	*tmp;

	bta_t		*img;

	cnvList_t	*bpl;
	bt_bigpic_t	*bp;

	lowfp	= xfopen(mkBardThreePath("LOW.PIC"), "rb");
	hifp	= xfopen(mkBardThreePath("HI.PIC"), "rb");

	bpl = bigpicList_new();

	for (i = 0; i < NPIC_TOTAL; i++) {
		picBuf = (uint8_t *)xzalloc(PICBUF_SIZE);

		if (pics[i].hiflag)
			curfp = hifp;
		else
			curfp = lowfp;

		fp_moveToIndex32(curfp, pics[i].index, 0);
		xfread(picBuf, 1, PICBUF_SIZE, curfp);
		data = d3comp(picBuf, PICBUF_SIZE);

		bp = bigpic_new();
		bp->name = getPicMacro(&pics[i]);
		bp->type = IMAGE_BTA;
		bp->path = mkImagePathRel("bigpic_%d.bta", i);
		cnvList_add(bpl, bp);

		img = bta_new(1);
		bta_loop_new(img, 0, 4);

		for (j = 0; j < 4; j++) {
			bta_cell_t *c;

			c = bta_cell_new(0, 0, 56, 88, 1, NULL);

			if (j) {
				uint32_t x;

				tmp = bts_ncopy(data, 4928, j * 4928);
				for (x = 0; x < 4928; x++)
					bbuf->buf[x] ^= tmp->buf[x];
				bts_free(tmp);
			} else {
				bbuf = bts_ncopy(data, 4928, j * 4928);
			}

			c->gfx = bts_strdup(bbuf);
			c->delay = 4;
			bta_cell_convert(c);
#ifdef DEBUG
			bta_toPNG(c, bts_sprintf("bigpic_%d.png", i));
#endif
			bta_cell_set(img, 0, j, c);
		}

		bta_write_bta(mkImagePath("bigpic_%d.bta", i), img);

		bts_free(bbuf);
		bts_free(data);
		bta_free(img);

		free(picBuf);
	}

	bigpicList_toJson(bpl, mkJsonPath("bigpic.json"));
	cnvList_free(bpl);

	fclose(lowfp);
	fclose(hifp);
}
