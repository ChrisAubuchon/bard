#include <b3lib.h>
#include <gfx.h>
#include <bt_gfx.h>
#include <cnv_gfx.h>
#include <dehuf.h>

#define DEBUG 1
#include <debug.h>

/********************************/
/*				*/
/* Static Variables		*/
/*				*/
/********************************/

iconGfx_t icons[] = {
	{0x474, "icon_light",    12, 19, 2},
	{0x820, "icon_compass",  16, 26, 0},
	{0x550, "icon_detect",   16, 17, 4},
	{0x1e0, "icon_shield",   12, 20, 0},
	{0x640, "icon_levitate", 20, 16, 3}
};

/********************************/
/*				*/
/* Private Function Prototypes	*/
/*				*/
/********************************/

static void outputScreens(void);
static void outputIcons(void);
static void outputMousePointers(void);

static void icon_write_bta(huffile_t *huf, int index);
static void icon_write_compass(huffile_t *data, int index);
static void icon_write_detect(huffile_t *huf, int index);
static void icon_write_png(huffile_t *huf, int index);
static void __icon_write_png(btstring_t *datap, btstring_t *fname,
				iconGfx_t *ip);

/********************************/
/*				*/
/* Private Functions		*/
/*				*/
/********************************/

static void outputScreens(void)
{
	uint8_t i;
	uint8_t *fnames[] = { "TITLE", "BARDSCR", "TIT0", "VICT" };
	uint8_t *tags[] = { "title", "main_screen", "tit0", "victory" };
	btstring_t *data;

	bta_cell_t *img;

	for (i = 0; i < 4; i++) {
		img = bta_cell_new(0, 0, 160, 200, 0, NULL);

		data = d3compFile(mkBardThreePath("%s", fnames[i]), 33000);
		img->gfx = bts_ncopy(data, data->size - 0x0d, 0x0d);
		bts_free(data);

		img = bta_cell_convert(img);
		bta_toPNG(img, mkImagePath("%s.png", tags[i]));

		bta_cell_free(img);
	}
}

/*
 * __icon_write_png()
 */
static void __icon_write_png(btstring_t *data, btstring_t *fname,
				iconGfx_t *ip)
{
	bta_cell_t	*img;

	img = bta_cell_new(0, 0, ip->width, ip->height, 0, NULL);
	img->gfx = bts_copy(data);
	img = bta_cell_convert(img);
	bta_toPNG(img, fname);
	bta_cell_free(img);
}

/*
 * icon_write_bta()
 */
static void icon_write_bta(huffile_t *huf, int index)
{
	bta_t		*bta;
	bta_loop_t	*l;
	bta_cell_t	*img;
	uint32_t	offset, size, ncells;
	iconGfx_t	*ip;
	btstring_t	*data;
	int		i, j;

	bta = bta_new(1);

	ip	= &icons[index];
	offset	= 0;
	size	= icons[index].height * icons[index].width;
	data	= dehuf(huf, icons[index].size);
	ncells	= (icons[index].size / size) - 1;
	l	= bta_loop_new(bta, 0, ncells);

	for (i = 0; i < ncells; i++) {
		img	= bta_cell_new(0, 0, 
				icons[index].width, icons[index].height, 
				icons[index].delay, NULL
				);
		img->gfx = bts_ncopy(data, size, offset);
		img = bta_cell_convert(img);
		bta_cell_set(bta, 0, i, img);

		offset += size;
	}

	bta_write_bta(mkImagePath("%s.bta", icons[index].name), bta);

	__icon_write_png(
		bts_ncopy(data, size, offset),
		mkImagePath("%s_clear.png", ip->name),
		ip
		);


	bta_free(bta);
	bts_free(data);

}

static uint8_t *cdir[] = {
	"north", "east", "south", "west", "clear"
};

/*
 * icon_write_compass()
 */
static void icon_write_compass(huffile_t *huf, int index)
{
	bta_cell_t	*img;
	int		i;
	uint32_t	offset, size;
	btstring_t	*data;
	iconGfx_t	*ip;

	ip = &icons[index];

	offset	= 0;
	size 	= ip->height * ip->width;
	data	= dehuf(huf, ip->size);

	for (i = 0; i < ip->size / size; i++) {
		img = bta_cell_new(0, 0, ip->width, ip->height, 0,
				bts_ncopy(data, size, offset));
		img = bta_cell_convert(img);
		bta_toPNG(img, mkImagePath("%s_%s.png", ip->name, cdir[i]));
		bta_cell_free(img);
		offset += size;
	}

	bts_free(data);
}

/*
 * icon_write_detect()
 */
static void icon_write_detect(huffile_t *huf, int index)
{
	bta_t		*bta;
	bta_loop_t	*l;
	bta_cell_t	*img;
	uint32_t	offset, size;
	btstring_t	*data;
	iconGfx_t	*ip;
	int		i, j;

	bta = bta_new(1);
	l = bta_loop_new(bta, 0, 6);

	ip	= &icons[index];
	offset	= 0;
	size	= ip->height * ip->width;
	data	= dehuf(huf, ip->size);

	for (i = 0; i < 6; i++) {
		if (i == 0) {
			img = bta_cell_new(0, 0, ip->width, ip->height, 
						35, NULL);
		} else {
			img = bta_cell_new(0, 0, ip->width, ip->height,
						ip->delay, NULL);
		}

		img->gfx = bts_ncopy(data, size, size * ((i > 3)? 6 - i : i));
		img = bta_cell_convert(img);
		bta_cell_set(bta, 0, i, img);
	}

	bta_write_bta(mkImagePath("%s.bta", ip->name), bta);

	__icon_write_png(
		bts_ncopy(data, size, size * 4), 
		mkImagePath("%s_clear.png", ip->name),
		ip
		);
	bta_free(bta);
	bts_free(data);
}

/*
 * icon_write_png()
 */
static void icon_write_png(huffile_t *huf, int index)
{
	bta_cell_t	*img;
	btstring_t	*data;
	iconGfx_t	*ip;
	uint32_t	size;

	ip = &icons[index];

	size = ip->width * ip->height;
	data = dehuf(huf, ip->size);
	__icon_write_png(
		bts_ncopy(data, size, 0),
		mkImagePath("%s.png", ip->name), 
		ip
		);
	__icon_write_png(
		bts_ncopy(data, size, size),
		mkImagePath("%s_clear.png", ip->name), 
		ip
		);

	bts_free(data);
}

/*
 * outputIcons()
 */
static void outputIcons(void)
{
	FILE		*fp;
	huffile_t	*huf;

	fp = xfopen(mkBardThreePath("ICONS.BIN"), "rb");
	huf = dehuf_init(fp);

	icon_write_bta(huf, 0);
	icon_write_compass(huf, 1);
	icon_write_detect(huf, 2);
	icon_write_png(huf, 3);
	icon_write_bta(huf, 4);

	dehuf_free(huf);
	fclose(fp);
}


#if 0
static void outputMousePointers(void)
{
	uint8_t i;
	btstring_t *data;
	bta_cell_t *img;

	for (i = 0; i < 7; i++) {
		data = bts_strncpy(&b3_mouseGfx[i][32], 128);
		data = gfx_vga2gfx(data);
		img = bta_cell_new(0, 0, 8, 16, 0, data);
		img = bta_cell_convert(img);
		bta_toPNG(img, bts_sprintf("mouse_%d.png", i));
		bta_cell_free(img);
	}
		
}
static void testRead(void)
{
	bt_array_t *cells;
	bta_t *bta;
	uint32_t i;

	bta = bta_read_bta(bts_strcpy("icon_light.bta"));
	cells = bt_array_get(bta->loops, 0);
	for (i = 0; i < bt_array_length(cells); i++) {
		bta_cell_t *c;

		c = bt_array_get(cells, i);
		bta_toPNG(c, bts_sprintf("icon_light_%d.png", i));
	}
}

#endif

/********************************/
/*				*/
/* Public Functions		*/
/*				*/
/********************************/

void convertGFX(void)
{
	outputScreens();
	outputIcons();
	outputBigpic();
#if 0
	outputTilepics();
	outputMousePointers();
#endif
}
