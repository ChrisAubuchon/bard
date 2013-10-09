#ifndef _CNV_GFX_H
#define _CNV_GFX_H

#include <bt_string.h>
/*#include <bt_gds.h>*/
#include <bt_bta.h>

#define COMPRESS_SIZE	128

#define GRID_CITY	1
#define GRID_DUN	2
#define GRID_WILD	3

#define QUAD_2M		0
#define QUAD_3M		1
#define QUAD_4M		2
#define QUAD_5M		3
#define QUAD_1L		4
#define QUAD_2L		5
#define QUAD_3L		6
#define QUAD_4L		7
#define QUAD_5L		8
#define QUAD_1R		9
#define QUAD_2R		10
#define QUAD_3R		11
#define QUAD_4R		12
#define QUAD_5R		13
#define QUAD_5FL	14
#define QUAD_5FR	15
#define QUAD_1M		16
#define QUAD_3FL	17
#define QUAD_4FL	18
#define QUAD_3FR	19
#define QUAD_4FR	20
#define QUAD_4WL	21
#define QUAD_5WL	22
#define QUAD_4WR	23
#define QUAD_5WR	24


/********************************/
/*				*/
/* Helpful macros		*/
/*				*/
/********************************/

#define bta_cell_convert(x)	bta_cell_scale(bta_cell_4bitTo8bit((x)))

/********************************/
/*				*/
/* Generic GFX Structures	*/
/*				*/
/********************************/

/*typedef struct bt_dllist_t bt_view_t;*/
typedef struct bt_view_t bt_view_t;
typedef struct bt_bigpicList_t bt_bigpicList_t;

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

bta_cell_t *bta_cell_scale(bta_cell_t *in);
bta_cell_t *bta_cell_4bitTo8bit(bta_cell_t *in);
bta_cell_t *bta_trim(bta_cell_t *in, uint16_t left, uint16_t right);
void bta_toPNG(bta_cell_t *in, btstring_t *fname);
void bta_transparent_toPNG(bta_cell_t *in, btstring_t *fname);

/* Bigpic list */
bt_bigpicList_t *bigpic_list_new(uint32_t size);
void bigpic_list_free(bt_bigpicList_t *bpl);
void bigpic_list_set(bt_bigpicList_t *bpl, btstring_t *name, \
		     btstring_t *path, uint8_t type);
void bigpic_list_to_json(bt_bigpicList_t *bpl, btstring_t *fname);

/* View list */
bt_view_t *bt_view_new(void);
void bt_view_free(bt_view_t *view);
void bt_view_new_facet(bt_view_t *view, btstring_t *quad, btstring_t *label, \
			uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void bt_view_to_json(bt_view_t *view, btstring_t *path);

#endif
