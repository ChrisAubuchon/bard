#include <bt_btf.h>
#include <btlib.h>
#include <gl_xlist.h>
#include <gl_array_list.h>

#define DEBUG
#include <debug.h>

/********************************/
/*				*/
/* Internal structure		*/
/*				*/
/********************************/

struct btf_charData_t {
	gl_list_t	varWidths;
	gl_list_t	charMaps;
};

/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

static void btf_cell_free(void *vbtfc);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

static void btf_cell_free(void *vbtfc)
{
	btstring_t *btfc = vbtfc;

	if (btfc)
		bts_free(btfc);
}


/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

btf_t *btf_new(uint16_t nchars, uint8_t mono_width, uint8_t height)
{
	btf_t *new;
	uint32_t i;
	void *data;

	new = (btf_t *)xzalloc(sizeof(btf_t));
	new->monoWidth = mono_width;
	new->height = height;

	data = xzalloc(nchars * sizeof(void *));

	new->charData = (btf_charData_t *)xzalloc(sizeof(btf_charData_t));

	new->charData->charMaps = gl_list_create(GL_ARRAY_LIST,
		NULL, NULL, bts_free, 1,
		nchars, data);
	new->charData->varWidths = gl_list_create(GL_ARRAY_LIST,
		NULL, NULL, NULL, 1,
		nchars, data);

	free(data);

	return new;
}

void btf_free(btf_t *btf)
{
	if (btf == NULL)
		return;

	gl_list_free(btf->charData->varWidths);
	gl_list_free(btf->charData->charMaps);
	free(btf->charData);
	free(btf);
}

void btf_write_btf(btstring_t *fname, btf_t *btf)
{
	FILE *fp;
	uint32_t i;
	uint32_t size;

	fp = xfopen(fname, "wb");

	size = gl_list_size(btf->charData->charMaps);

	fp_write32le(fp, size);
	xfwrite(&btf->height, sizeof(uint8_t), 1, fp);
	xfwrite(&btf->monoWidth, sizeof(uint8_t), 1, fp);

	size = btf->height * btf->monoWidth;

	for (i = 0; i < gl_list_size(btf->charData->varWidths); i++) {
		uint32_t w;

		w = (uint32_t)gl_list_get_at(btf->charData->varWidths, i);

		xfwrite(&w, sizeof(uint32_t), 1, fp);
	}

	for (i = 0; i < gl_list_size(btf->charData->charMaps); i++) {
		btstring_t *bs;

		if (gl_list_get_at(btf->charData->charMaps, i) != NULL) {
			bs = (btstring_t *)gl_list_get_at(btf->charData->charMaps, i);

			xfwrite(bs->buf, sizeof(uint8_t), bs->size, fp);
		}
	}
	
	bts_free(fname);
	fclose(fp);
}

btf_t *btf_read_btf(btstring_t *fname)
{
	FILE *fp;
	btf_t *btf;
	uint32_t nchars;
	uint8_t height;
	uint8_t width;
	uint32_t size;
	uint32_t i;
	uint32_t c;

	fp = xfopen(fname, "rb");

	nchars = fp_read32le(fp);
	xfread(&height, sizeof(uint8_t), 1, fp);
	xfread(&width, sizeof(uint8_t), 1, fp);
	size = width * height;

	btf = btf_new(nchars, width, height);

	for (i = 0; i < nchars; i++)  {
		c = fp_read32le(fp);
		gl_list_set_at(btf->charData->varWidths, i, (void *)c);
	}

	for (i = 0; i < nchars; i++) {
		if (gl_list_get_at(btf->charData->varWidths, i) != 0) {
			btstring_t *bs = bts_new(size);

			xfread(bs->buf, sizeof(uint8_t), size, fp);

			gl_list_set_at(btf->charData->charMaps, i, bs);
		}
	}

	fclose(fp);
	bts_free(fname);

	return btf;
}

void btf_setChar(btf_t *btf, uint16_t index, uint32_t varWidth, btstring_t *map)
{
	gl_list_set_at(btf->charData->varWidths, index, (void *)varWidth);
	gl_list_set_at(btf->charData->charMaps, index, map);
}

void btf_getChar(btf_t *btf, uint16_t index, uint32_t *varWidth, btstring_t **map)
{
	*varWidth = (uint32_t)gl_list_get_at(btf->charData->varWidths, index);
	*map = (btstring_t *)gl_list_get_at(btf->charData->charMaps, index);
}

uint32_t btf_getNumChars(btf_t *btf)
{
	return gl_list_size(btf->charData->charMaps);
}

/********************************/
/*				*/
/* Library interface		*/
/*				*/
/********************************/

btf_t *BTF_OpenFont(btstring_t *fname, uint8_t flags)
{
	btf_t *rval;

	rval = btf_read_btf(fname);
	rval->type = flags;

	return rval;
}

btf_t *BTF_CloseFont(btf_t *btf)
{
	btf_free(btf);
}

uint8_t BTF_FontHeight(btf_t *btf)
{
	return btf->height;
}


int BTF_SizeText(btf_t *btf, const char *text, int *w, int *h)
{
	const char *textp = text;

	*h = btf->height;
	*w = 0;

	if (btf->type == BTF_MONOSPACE) {
		*w = strlen(text) * btf->monoWidth;
	} else {
		while (*textp != '\0') {
			*w += (uint32_t)gl_list_get_at(btf->charData->varWidths, *textp);
			textp++;
		}
	}

	return 0;
}
