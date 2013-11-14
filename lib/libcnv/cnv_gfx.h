#ifndef _CNV_GFX_H
#define _CNV_GFX_H

#include <bt_string.h>
#include <bt_bta.h>

/********************************/
/*				*/
/* Helpful macros		*/
/*				*/
/********************************/

#define	IMAGE_PNG		1
#define IMAGE_BTA		2

#define bta_cell_convert(x)	bta_cell_toRGBA(bta_cell_scale(bta_cell_4bitTo8bit((x))), egapal)

/********************************/
/*				*/
/* Generic GFX Structures	*/
/*				*/
/********************************/

typedef struct {
	uint8_t		red;
	uint8_t		green;
	uint8_t		blue;
	uint8_t		alpha;
} bta_color_t;

typedef struct bt_view_t bt_view_t;
typedef struct bt_bigpicList_t bt_bigpicList_t;

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

extern 		bta_color_t egapal[];

bta_cell_t	*bta_cell_scale(bta_cell_t *in);
bta_cell_t	*bta_cell_4bitTo8bit(bta_cell_t *in);
bta_cell_t	*bta_trim(bta_cell_t *in, uint16_t left, uint16_t right);
bta_cell_t	*bta_cell_toRGBA(bta_cell_t *in, bta_color_t *pal);
void bta_toPNG(bta_cell_t *in, btstring_t *fname);
void bta_transparent_toPNG(bta_cell_t *in, btstring_t *fname);

#if 0
/* Bigpic list */
bt_bigpicList_t *bigpic_list_new(uint32_t size);
void bigpic_list_free(bt_bigpicList_t *bpl);
void bigpic_list_set(bt_bigpicList_t *bpl, btstring_t *name, \
		     btstring_t *path, uint8_t type);
void bigpic_list_to_json(bt_bigpicList_t *bpl, btstring_t *fname);
#endif

/* View list */
bt_view_t *bt_view_new(void);
void bt_view_free(bt_view_t *view);
void bt_view_new_facet(bt_view_t *view, btstring_t *quad, btstring_t *label, \
			uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void bt_view_to_json(bt_view_t *view, btstring_t *path);

#endif
