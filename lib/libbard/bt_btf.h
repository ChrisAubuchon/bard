#ifndef _BT_BTF_H
#define _BT_BTF_H

/*
 * bt_btf.h - Bard's Tale Font file format
 */

#include <stdint.h>
#include <bt_string.h>

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define BTF_MONOSPACE	1
#define BTF_VARWIDTH	2

/********************************/
/*				*/
/* 				*/
/*				*/
/********************************/

typedef struct btf_charData_t btf_charData_t;

typedef struct {
	uint32_t 	type;
	uint32_t	height;
	uint32_t	monoWidth;
	btf_charData_t	*charData;
} btf_t;


/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

btf_t *btf_new(uint16_t nchars, uint8_t mono_width, uint8_t height);
void btf_free(btf_t *btf);

void btf_setChar(btf_t *btf, uint16_t index, uint32_t varWidth, btstring_t *map);
void btf_getChar(btf_t *btf, uint16_t index, uint32_t *varwidth, btstring_t **map);
uint32_t btf_getNumChars(btf_t *btf);

void btf_write_btf(btstring_t *fname, btf_t *btf);
btf_t *btf_read_btf(btstring_t *fname);


/********************************/
/*				*/
/* Library interface		*/
/*				*/
/********************************/

btf_t *BTF_OpenFont(btstring_t *name, uint8_t flags);
btf_t *BTF_CloseFont(btf_t *btf);
uint8_t BTF_FontHeight(btf_t *btf);
int BTF_SizeText(btf_t *btf, const char *text, int *w, int *h);

#endif
