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

#define bta_cell_convert(x)	\
	do {								\
		bta_cell_4bitTo8bit((x));				\
		bta_cell_scale((x));					\
		bta_cell_toRGBA((x), egapal);				\
	} while (0)

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

void	bta_cell_scale(bta_cell_t *in);
void	bta_cell_4bitTo8bit(bta_cell_t *in);
void	bta_trim(bta_cell_t *in, uint16_t left, uint16_t right);
void	bta_cell_toRGBA(bta_cell_t *in, bta_color_t *pal);
void	bta_toPNG(bta_cell_t *in, btstring_t *fname);
void	bta_transparent_toPNG(bta_cell_t *in, btstring_t *fname);

/* View list */
bt_view_t *bt_view_new(void);
void bt_view_free(bt_view_t *view);
void bt_view_new_facet(bt_view_t *view, btstring_t *quad, btstring_t *label, \
			uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void bt_view_to_json(bt_view_t *view, btstring_t *path);

#endif
