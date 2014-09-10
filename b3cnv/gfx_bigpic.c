#include <b3lib.h>
#include <gfx.h>
#include <cnv_gfx.h>
#include <cnv_bigpic.h>
#include <dehuf.h>

#define DEBUG 
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
/*  0*/ {0, 0}, {0, 1}, {0, 2}, {1, 91},
/*  4*/ {0, 4}, {1, 93}, {0, 6}, {1, 95},
/*  8*/ {0, 8}, {1, 97}, {1, 98}, {0, 11},
/* 12*/ {0, 12}, {0, 13}, {1, 102}, {1, 103},
/* 16*/ {0, 16}, {0, 17}, {0, 18}, {0, 19},
/* 20*/ {0, 20}, {1, 109}, {1, 110}, {1, 111},
/* 24*/ {0, 24}, {1, 113}, {0, 26}, {1, 116},
/* 28*/ {0, 29}, {0, 30}, {1, 119}, {1, 120},
/* 32*/ {0, 33}, {1, 122}, {1, 123}, {1, 124},
/* 36*/ {1, 125}, {1, 126}, {0, 39}, {1, 128},
/* 40*/ {1, 129}, {1, 130}, {0, 43}, {1, 132},
/* 44*/ {1, 133}, {0, 46}, {0, 47}, {1, 136},
/* 48*/ {0, 49}, {0, 50}, {1, 139}, {1, 140},
/* 52*/ {1, 141}, {1, 142}, {1, 143}, {0, 56},
/* 56*/ {0, 57}, {0, 58}, {0, 59}, {1, 148},
/* 60*/ {1, 149}, {1, 150}, {0, 63}, {1, 152},
/* 64*/ {1, 153}, {1, 154}, {1, 155}, {1, 156},
/* 68*/ {0, 69}, {0, 70}, {0, 71}, {1, 160},
/* 72*/ {1, 161}, {0, 74}, {1, 163}, {1, 164},
/* 76*/ {1, 165}, {1, 166}, {1, 167}, {1, 168},
/* 80*/ {1, 170}, {0, 83}, {1, 172}, {1, 173},
/* 84*/ {1, 175}
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

		fp_moveToIndex32(curfp, getPicIndex(pics[i]), 0);
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
