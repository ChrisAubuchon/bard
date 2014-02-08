#include <b1lib.h>
#include <gfx.h>
#include <bt_gfx.h>
#include <dehuf.h>
#include <cnv_gfx.h>
#include <fpio.h>

#define DEBUG 1
#include <debug.h>

/****************************************/
/*					*/
/* Static Variables			*/
/*					*/
/****************************************/

iconGfx_t icons[] = {
	{0x5f0, "icon_light", 20, 19, 2},
	{0x6e0, "icon_levitate", 20, 22, 4},
	{0x7d0, "icon_compass", 20, 25, 0},
	{0x460, "icon_detect", 20, 14, 4},
	{0x154, "icon_shield", 20, 17, 0}
};

/****************************************/
/*					*/
/* Output Function Prototypes		*/
/*					*/
/****************************************/
static void outputScreens(uint8_t indent);
static void outputIcons(uint8_t indent);

static void icon_write_bta(huffile_t *huf, int index);
static void icon_write_compass(huffile_t *huf, int index);
static void icon_write_detect(huffile_t *huf, int index);
static void icon_write_png(huffile_t *huf, int index);
static void icon_write_clear(btstring_t *data, int index);

/****************************************/
/*					*/
/* Helper Functions			*/
/*					*/
/****************************************/

/****************************************/
/*					*/
/* Output Functions			*/
/*					*/
/****************************************/

/*
 * outputScreens()
 */
static void outputScreens(uint8_t indent)
{
	uint8_t i;
	uint8_t *fnames[] = { "BARDTIT", "BARDSCR" };
	uint8_t *tags[] = { "title", "screen" };

	bta_cell_t *img;

	for (i = 0; i < 2; i++) {
		img = bta_cell_new(0, 0, 160, 200, 0, 
			gfx_vga2gfx(dehufFile(mkBardOnePath("%s", fnames[i]), 0x7d00)));
		img = bta_cell_convert(img);
		bta_toPNG(img, mkImagePath("%s.png", tags[i]));
		bta_cell_free(img);
	}
}

/*
 * icon_write_detect()
 */
static void icon_write_detect(huffile_t *huf, int index)
{
	bta_t *bta;
	bta_loop_t *l;
	bta_cell_t *img;
	uint32_t offset, size;
	int i, j;
	btstring_t *data;

	bta = bta_new(1);
	l = bta_loop_new(bta, 0, 6);

	offset = 0;
	size = icons[index].height * icons[index].width;
	data = dehuf(huf, icons[index].size);

	for (i = 0; i < 6; i++) {
		if (i == 0)
			img = bta_cell_new(0, 0, 
				icons[index].width, icons[index].height, 
				25, NULL);
		else
			img = bta_cell_new(0, 0, 
				icons[index].width, icons[index].height, 
				icons[index].delay, NULL);

		img->gfx = bts_ncopy(data, size, size * ((i > 3) ? 6 - i : i));
		img = bta_cell_convert(img);
		bta_cell_set(bta, 0, i, img);
	}

	bta_write_bta(mkImagePath("%s.bta", icons[index].name), bta);

	icon_write_clear(bts_ncopy(data, size, 0), index);

	bta_free(bta);
	bts_free(data);
}

/*
 * icon_write_bta()
 */
static void icon_write_bta(huffile_t *huf, int index)
{
	bta_t *bta;
	bta_loop_t *l;
	bta_cell_t *img;
	uint32_t offset, size;
	uint32_t ncells;
	int i, j;
	btstring_t *data;

	bta = bta_new(1);

	offset = 0;
	size = icons[index].height * icons[index].width;
	data = dehuf(huf, icons[index].size);
	ncells = icons[index].size / size;
	l = bta_loop_new(bta, 0, ncells);

	for (i = 0; i < ncells; i++) {
		img = bta_cell_new(0, 0, 
			icons[index].width, icons[index].height,
			icons[index].delay, NULL);

		img->gfx = bts_ncopy(data, size, offset);
		img = bta_cell_convert(img);
		bta_cell_set(bta, 0, i, img);

		offset += size;
	}

	bta_write_bta(mkImagePath("%s.bta", icons[index].name), bta);

	icon_write_clear(bts_ncopy(data, size, 0), index);

	bta_free(bta);
	bts_free(data);
}

/*
 * icon_write_clear()
 */
static void icon_write_clear(btstring_t *data, int index)
{
	bta_cell_t *img;
	int i, j;

	img = bta_cell_new(0, 0, 
		icons[index].width, icons[index].height, 
		0, NULL);

	img->gfx = data;
	for (i = 0; i < icons[index].height; i++) {
		for (j = 2; j < icons[index].width; j++) {
			img->gfx->buf[(i * icons[index].width) + j] = 0x77;
		}
	}
	img = bta_cell_convert(img);
	bta_toPNG(img, mkImagePath("%s_clear.png", icons[index].name));
	bta_cell_free(img);
}

/*
 * icon_write_png()
 */
static void icon_write_png(huffile_t *huf, int index)
{
	bta_cell_t *img;
	btstring_t *data;

	data = dehuf(huf, icons[index].size);

	img = bta_cell_new(0, 0,
		icons[index].width, icons[index].height,
		0, NULL);

	img->gfx = bts_copy(data);
	img = bta_cell_convert(img);
	bta_toPNG(img, mkImagePath("%s.png", icons[index].name));

	icon_write_clear(bts_copy(data), index);

	bta_cell_free(img);
	bts_free(data);
}

static uint8_t *cdir[] = {
	"north", "east", "south", "west"
};

/*
 * icon_write_compass()
 */
static void icon_write_compass(huffile_t *huf, int index)
{
	bta_cell_t *img;
	int i;
	uint32_t offset, size;
	btstring_t *data;

	offset = 0;
	size = icons[index].height * icons[index].width;
	data = dehuf(huf, icons[index].size);

	for (i = 0; i < icons[index].size / size; i++) {
		img = bta_cell_new(0, 0,
			icons[index].width, icons[index].height,
			0, bts_ncopy(data, size, offset));
		img = bta_cell_convert(img);
		bta_toPNG(img, mkImagePath("%s_%s.png",
				icons[index].name, cdir[i]));
		bta_cell_free(img);

		offset += size;
	}

	icon_write_clear(bts_ncopy(data, size, 0), index);
}

/*
 * outputIcons()
 */
static void outputIcons(uint8_t indent)
{
	FILE *fp;
	huffile_t *huf;

	fp = xfopen(mkBardOnePath("icons.bin"), "rb");
	huf = dehuf_init(fp);

	icon_write_bta(huf, 0);
	icon_write_bta(huf, 1);

	icon_write_compass(huf, 2);
	icon_write_detect(huf, 3);

	icon_write_png(huf, 4);

	dehuf_free(huf);
	fclose(fp);
}

void convertGFX(void)
{
	outputScreens(2);
	outputIcons(2);
	outputBigpic(2);
	outputBitmapFont();
	outputCitypics(2);
	outputDunpics(2);
}
