#include <b1lib.h>
#include <bt_bta.h>
#include <bt_gfx.h>
#include <cnv_gfx.h>
#include <gfx.h>
#include <bt_btf.h>

/*#define DEBUG 1*/
#include <debug.h>

static uint8_t bitmap[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0x60, 0xF0, 0xF0, 0x60, 0x60, 0, 0x60, 0,
  0xD8, 0xD8, 0xD8, 0, 0, 0, 0, 0,
  0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0,
  0x30, 0x7C, 0xC0, 0x78, 0x0C, 0xF8, 0x30, 0,
  0, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xC6, 0,
  0x38, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0x76, 0,
  0x60, 0x60, 0xC0, 0, 0, 0, 0, 0,
  0x30, 0x60, 0xC0, 0xC0, 0xC0, 0x60, 0x30, 0,
  0xC0, 0x60, 0x30, 0x30, 0x30, 0x60, 0xC0, 0,
  0, 0xCC, 0x78, 0xFC, 0x78, 0xCC, 0, 0,
  0, 0x30, 0x30, 0xFC, 0x30, 0x30, 0, 0,
  0, 0, 0, 0, 0, 0x60, 0x60, 0xC0,
  0, 0, 0, 0xF8, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0x60, 0x60, 0,
  0, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0,
  0x7C, 0xC6, 0xCE, 0xDE, 0xF6, 0xE6, 0x7C, 0,
  0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xFC, 0,
  0x78, 0xCC, 0x0C, 0x38, 0x60, 0xCC, 0xFC, 0,
  0x78, 0xCC, 0x0C, 0x38, 0x0C, 0xCC, 0x78, 0,
  0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x1E, 0,
  0xFC, 0xC0, 0xF8, 0x0C, 0x0C, 0xCC, 0x78, 0,
  0x38, 0x60, 0xC0, 0xF8, 0xCC, 0xCC, 0x78, 0,
  0xFC, 0xCC, 0x0C, 0x18, 0x30, 0x30, 0x30, 0,
  0x78, 0xCC, 0xCC, 0x78, 0xCC, 0xCC, 0x78, 0,
  0x78, 0xCC, 0xCC, 0x7C, 0x0C, 0x18, 0x70, 0,
  0, 0xC0, 0xC0, 0, 0, 0xC0, 0xC0, 0,
  0, 0x60, 0x60, 0, 0, 0x60, 0x60, 0xC0,
  0x18, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x18, 0,
  0, 0, 0xF8, 0, 0, 0xF8, 0, 0,
  0xC0, 0x60, 0x30, 0x18, 0x30, 0x60, 0xC0, 0,
  0x78, 0xCC, 0x0C, 0x18, 0x30, 0, 0x30, 0,
  0x7C, 0xC6, 0xDE, 0xDE, 0xDE, 0xC0, 0x78, 0,
  0x30, 0x78, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0,
  0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0,
  0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0,
  0xF8, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0,
  0xFE, 0x62, 0x68, 0x78, 0x68, 0x62, 0xFE, 0,
  0xFE, 0x62, 0x68, 0x78, 0x68, 0x60, 0xF0, 0,
  0x3C, 0x66, 0xC0, 0xC0, 0xCE, 0x66, 0x3E, 0,
  0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0,
  0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0,
  0x1E, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0,
  0xE6, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6, 0,
  0xF0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0,
  0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0,
  0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0,
  0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0,
  0xFC, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0,
  0x78, 0xCC, 0xCC, 0xCC, 0xDC, 0x78, 0x1C, 0,
  0xFC, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0xE6, 0,
  0x78, 0xCC, 0xE0, 0x70, 0x1C, 0xCC, 0x78, 0,
  0xFC, 0xB4, 0x30, 0x30, 0x30, 0x30, 0x78, 0,
  0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0,
  0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0,
  0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0xEE, 0xC6, 0,
  0xC6, 0xC6, 0x6C, 0x38, 0x38, 0x6C, 0xC6, 0,
  0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x30, 0x78, 0,
  0xFE, 0xC6, 0x8C, 0x18, 0x32, 0x66, 0xFE, 0,
  0xF0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xF0, 0,
  0, 0xC0, 0x60, 0x30, 0x18, 0x0C, 4, 0,
  0xF0, 0x30, 0x30, 0x30, 0x30, 0x30, 0xF0, 0,
  0x10, 0x38, 0x6C, 0xC6, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0xFF,
  0xC0, 0xC0, 0x60, 0, 0, 0, 0, 0,
  0, 0, 0xF0, 0x18, 0xF8, 0x98, 0xE8, 0,
  0xC0, 0xC0, 0xC0, 0xF0, 0xD8, 0xD8, 0xF0, 0,
  0, 0, 0xF0, 0xD0, 0xC0, 0xD0, 0xF0, 0,
  0x38, 0x18, 0x18, 0x78, 0xD8, 0xD8, 0x78, 0,
  0, 0, 0x70, 0xD8, 0xF8, 0xC0, 0x70, 0,
  0x38, 0x68, 0x60, 0xF0, 0x60, 0x60, 0xF0, 0,
  0, 0, 0x68, 0xD8, 0xD8, 0xF8, 0x18, 0xF0,
  0xC0, 0xC0, 0xC0, 0xF0, 0xD8, 0xD8, 0xD8, 0,
  0xC0, 0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0,
  0x18, 0, 0x18, 0x18, 0x18, 0xD8, 0xD8, 0xF0,
  0xC0, 0xC0, 0xC8, 0xD8, 0xF0, 0xD8, 0xC8, 0,
  0xE0, 0x60, 0x60, 0x60, 0x60, 0x60, 0xF0, 0,
  0, 0, 0xCC, 0xFE, 0xFE, 0xD6, 0xC6, 0,
  0, 0, 0xF0, 0xD8, 0xD8, 0xD8, 0xD8, 0,
  0, 0, 0x70, 0xD8, 0xD8, 0xD8, 0x70, 0,
  0, 0, 0x70, 0xD8, 0xD8, 0xF0, 0xC0, 0xC0,
  0, 0, 0x78, 0xD8, 0xD8, 0x78, 0x18, 0x38,
  0, 0, 0x70, 0xD8, 0xC0, 0xC0, 0xE0, 0,
  0, 0, 0x78, 0xC0, 0x70, 0x18, 0xF0, 0,
  0x20, 0x60, 0xF8, 0x60, 0x60, 0x68, 0x30, 0,
  0, 0, 0xD8, 0xD8, 0xD8, 0xD8, 0xF8, 0,
  0, 0, 0xD8, 0xD8, 0xD8, 0x70, 0x20, 0,
  0, 0, 0x88, 0xA8, 0xA8, 0xA8, 0xF8, 0,
  0, 0, 0x88, 0xD8, 0x70, 0xD8, 0x88, 0,
  0, 0, 0xD8, 0xD8, 0xD8, 0xF8, 0x18, 0xF0,
  0, 0, 0xF8, 0x30, 0x60, 0xC8, 0xF8, 0,
  0x38, 0x60, 0x60, 0xC0, 0x60, 0x60, 0x38, 0,
  0xC0, 0xC0, 0xC0, 0, 0xC0, 0xC0, 0xC0, 0,
  0xE0, 0x30, 0x30, 0x18, 0x30, 0x30, 0xE0, 0,
  0x76, 0xDC, 0, 0, 0, 0, 0, 0,
  0, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0,
  0x7E, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7E,
  0x18, 0x3C, 0x7E, 0xFF, 0x18, 0x18, 0x18, 0x18,
  0x18, 0x18, 0x18, 0x18, 0xFF, 0x7E, 0x3C, 0x18,
  0, 0, 0x78, 0xCC, 0xC0, 0xCC, 0x78, 0,
  0x1C, 0x0C, 0x0C, 0x7C, 0xCC, 0xCC, 0x76, 0,
  0, 0, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0,
  0x38, 0x6C, 0x60, 0xF0, 0x60, 0x60, 0xF0, 0,
  0, 0, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8,
  0xE0, 0x60, 0x6C, 0x76, 0x66, 0x66, 0xE6, 0,
  0x30, 0, 0x70, 0x30, 0x30, 0x30, 0x78, 0,
  0x0C, 0, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78,
  0xE0, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0xE6, 0,
  0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0,
  0, 0, 0xCC, 0xFE, 0xFE, 0xD6, 0xC6, 0,
  0, 0, 0xF8, 0xCC, 0xCC, 0xCC, 0xCC, 0,
  0, 0, 0x78, 0xCC, 0xCC, 0xCC, 0x78, 0,
  0, 0, 0xDC, 0x66, 0x66, 0x7C, 0x60, 0xF0,
  0, 0, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0x1E,
  0, 0, 0xDC, 0x76, 0x66, 0x60, 0xF0, 0,
  0, 0, 0x7C, 0xC0, 0x78, 0x0C, 0xF8, 0,
  0x10, 0x30, 0x7C, 0x30, 0x30, 0x34, 0x18, 0,
  0, 0, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0,
  0, 0, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0,
  0, 0, 0xC6, 0xD6, 0xFE, 0xFE, 0x6C, 0,
  0, 0, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0,
  0, 0, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8,
  0, 0, 0xFC, 0x98, 0x30, 0x64, 0xFC, 0,
  0x1C, 0x30, 0x30, 0xE0, 0x30, 0x30, 0x1C, 0,
  0x18, 0x18, 0x18, 0, 0x18, 0x18, 0x18, 0,
  0xE0, 0x30, 0x30, 0x1C, 0x30, 0x30, 0xE0, 0,
  0x76, 0xDC, 0, 0, 0, 0, 0, 0,
  0, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0
};


void outputBitmapFont(void)
{
	btf_t *btf;
	btstring_t *cell;
	bta_cell_t *img;
	uint32_t i, j;
	uint8_t c;
	uint32_t varWidth;

	btf = btf_new(256, 16, 16);
	for (i = 1; i < 129; i++) {
		img = bta_cell_new(0, 0, 8, 8, 0, NULL);
		cell = bts_new(64);

		for (j = 0; j < 8; j++) {
			c = bitmap[(i * 8) + j];

			cell->buf[(j * 8) + 0] = (c & 0x80) ? 0x01 : 0;
			cell->buf[(j * 8) + 1] = (c & 0x40) ? 0x01 : 0;
			cell->buf[(j * 8) + 2] = (c & 0x20) ? 0x01 : 0;
			cell->buf[(j * 8) + 3] = (c & 0x10) ? 0x01 : 0;
			cell->buf[(j * 8) + 4] = (c & 0x08) ? 0x01 : 0;
			cell->buf[(j * 8) + 5] = (c & 0x04) ? 0x01 : 0;
			cell->buf[(j * 8) + 6] = (c & 0x02) ? 0x01 : 0;
			cell->buf[(j * 8) + 7] = (c & 0x01) ? 0x01 : 0;
		}

		img->gfx = cell;
		img = bta_cell_scale(img);

#if DEBUG
		btstring_t *fname;
		fname = bts_sprintf("char_%d.gfx", i + ' ');
		dump_btstring(fname, img->gfx, 0);
		bts_free(fname);
#endif

		c = i + ' ';
		if (c == 'i') {
			varWidth = 6;
		} else if ((islower(c)) && (c != 'm')) {
			varWidth = 12;
		} else {
			varWidth = 16;
		}


		btf_setChar(btf, i + ' ', varWidth, bts_copy(img->gfx));

		bta_cell_free(img);
	}

	/* Special handling for the ' ' character */
	cell = bts_new(64);
	for (i = 0; i < 64; i++)
		cell->buf[i] = 0;
	img = bta_cell_new(0, 0, 8, 8, 0, NULL);
	img->gfx = cell;
	img = bta_cell_scale(img);
	btf_setChar(btf, ' ', 12, bts_copy(img->gfx));
	bta_cell_free(img);

	btf_write_btf(mkFontPath("bt1font.btf"), btf);

	btf_free(btf);
}
