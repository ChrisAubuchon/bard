#include <bt_bta.h>
#include <btlib.h>
#include <bt_zlib.h>
#include <gl_xlist.h>
#include <gl_array_list.h>
#include <assert.h>

/*#define DEBUG*/
#include <debug.h>

/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

static void _bta_write_cell(FILE *fp, bta_cell_t *btac);
static void _bta_write_loops(FILE *fp, bta_t *bta);
static bta_t *_bta_read_loops(FILE *fp);
static bta_cell_t *_bta_read_cell(FILE *fp);

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/

static void _bta_write_cell(FILE *fp, bta_cell_t *btac)
{
	fp_write16le(fp, btac->x);
	fp_write16le(fp, btac->y);
	fp_write16le(fp, btac->width);
	fp_write16le(fp, btac->height);
	fp_write16le(fp, btac->delay);

	btac->gfx = zlib_compress(btac->gfx);

	/* Write the compressed size */
	fp_write32le(fp, btac->gfx->size);
	xfwrite(btac->gfx->buf, sizeof(uint8_t), btac->gfx->size, fp);
}

static bta_cell_t *_bta_read_cell(FILE *fp)
{
	bta_cell_t *rval;
	uint32_t size;
	uint32_t compsize;

	rval = (bta_cell_t *)xzalloc(sizeof(bta_cell_t));
	rval->x = fp_read16le(fp);
	rval->y = fp_read16le(fp);
	rval->width = fp_read16le(fp);
	rval->height = fp_read16le(fp);
	rval->delay = fp_read16le(fp);
	compsize = fp_read32le(fp);
	debug("x = %d\n", rval->x);
	debug("y = %d\n", rval->y);
	debug("width = %d\n", rval->width);
	debug("height = %d\n", rval->height);
	debug("compsize = %d\n", compsize);
	debug("size = %d\n", (rval->height * rval->width) * 4);
	size = (rval->height * rval->width) * 4;

	rval->gfx = bts_new(compsize);
	xfread(rval->gfx->buf, sizeof(uint8_t), compsize, fp);

	rval->gfx = zlib_uncompress(rval->gfx, size);

	return rval;
}

static void _bta_write_loops(FILE *fp, bta_t *bta)
{
	uint32_t lnum;
	uint32_t cnum;
	gl_list_t loops;
	gl_list_t cells;

	loops = (gl_list_t)bta->loopData;

	fp_write32le(fp, gl_list_size(loops));

	for (lnum = 0; lnum < gl_list_size(loops); lnum++) {
		bta_loop_t *l = (bta_loop_t *)gl_list_get_at(loops, lnum);

		cells = (gl_list_t)l->cellData;

		fp_write32le(fp, gl_list_size(cells));
		for (cnum = 0; cnum < gl_list_size(cells); cnum++) {
			_bta_write_cell(fp, (bta_cell_t *)gl_list_get_at(cells, cnum));
		}
	}
}

static bta_t *_bta_read_loops(FILE *fp)
{
	bta_loop_t	*l;
	uint32_t	nloops, ncells;
	uint32_t	lnum, cnum;
	bta_t	*rval;

	debug("ftell(fp) = %x\n", ftell(fp));
	lnum = ftell(fp);

	nloops = fp_read32le(fp);
	debug("nloops = %d\n", nloops);

	rval = bta_new(nloops);
	
	for (lnum = 0; lnum < nloops; lnum++) {

		ncells = fp_read32le(fp);

		l = bta_loop_new(rval, lnum, ncells);
		for (cnum = 0; cnum < ncells; cnum++) {
			gl_list_set_at(l->cellData, cnum, 
				_bta_read_cell(fp));
		}

	}

	return rval;
}

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

/*
 * bta_cell_new()
 */
bta_cell_t *bta_cell_new(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t delay, btstring_t *gfx)
{
	bta_cell_t *new;

	new = (bta_cell_t *)xzalloc(sizeof(bta_cell_t));

	new->x = x;
	new->y = y;
	new->width = width;
	new->height = height;
	new->delay = delay;
	new->gfx = gfx;

	return new;
}

/*
 * bta_cell_free()
 */
void bta_cell_free(const void *vbtac)
{
	bta_cell_t *btac = (bta_cell_t *)vbtac;

	if (btac == NULL)
		return;

	bts_free(btac->gfx);
	free(btac);
}

/*
 * bta_loop_new()
 */
bta_loop_t *bta_loop_new(bta_t *bta, uint32_t lnum, uint32_t ncells)
{
	bta_loop_t *new;
	bta_cell_t **cells;
	void *data;

	new = (bta_loop_t *)xzalloc(sizeof(bta_loop_t));
	data = xzalloc(ncells * sizeof(void *));
	new->cellData = gl_list_create(GL_ARRAY_LIST,
			NULL, NULL, bta_cell_free, 1,
			ncells, data);
	free(data);

	gl_list_set_at(bta->loopData, lnum, new);

	return new;
}

/*
 * bta_loop_free()
 */
void bta_loop_free(const void *vbtal)
{
	bta_loop_t *btal = (bta_loop_t *)vbtal;

	gl_list_free(btal->cellData);
	free(btal);
}

/*
 * bta_new()
 */
bta_t *bta_new(uint32_t nloops)
{
	bta_t *new;
	void *data;
	uint32_t i;

	new = (bta_t *)xzalloc(sizeof(bta_t));

	if (nloops) {
		data = xzalloc(nloops * sizeof(void *));
		new->loopData = gl_list_create(GL_ARRAY_LIST,
			NULL, NULL, bta_loop_free, 1,
			nloops, data);
		free(data);
	}
		
	return new;
}

/*
 * bta_free()
 */
void bta_free(bta_t *bta)
{
	bta_cell_free(bta->base);
	gl_list_free(bta->loopData);
	free(bta);
}

/*
 * bta_write_bta()
 */
void bta_write_bta(btstring_t *fname, bta_t *bta)
{
	FILE		*fp;
	uint16_t	flags = 0;

	fp = xfopen(fname, "wb");

	if (bta->base != NULL) 
		flags |= BTA_HASBASE;

	fp_write16le(fp, flags);
	_bta_write_loops(fp, bta);

	if (flags & BTA_HASBASE)
		_bta_write_cell(fp, bta->base);
	
	bts_free(fname);
	fclose(fp);
}

/*
 * bta_read_bta()
 */
bta_t *bta_read_bta(btstring_t *fname)
{
	FILE		*fp;
	bta_t		*rval;
	uint16_t	flags = 0;

	fp = xfopen(fname, "rb");

	flags = fp_read16le(fp);
	rval = _bta_read_loops(fp);

	if (flags & BTA_HASBASE)
		rval->base = _bta_read_cell(fp);

	bts_free(fname);
	fclose(fp);

	return rval;
}

/*
 * bta_cell_get()
 *
 * Return the cell data at loop->cell
 */
bta_cell_t *bta_cell_get(bta_t *bta, uint32_t loop, uint32_t cell)
{
	const bta_loop_t *l;

	l = gl_list_get_at((gl_list_t)bta->loopData, loop);
	if (l == NULL) 
		return NULL;
	return (bta_cell_t *)gl_list_get_at((gl_list_t)l->cellData, cell);
}

/*
 * bta_cell_set()
 *
 * Insert cell data at loop->cell
 */
void bta_cell_set(bta_t *bta, uint32_t lnum, uint32_t cnum, bta_cell_t *img)
{
	const bta_loop_t *l;

	l = gl_list_get_at((gl_list_t)bta->loopData, lnum);
	if (l == NULL) {
		bt_error("Loop out of range: %d", lnum);
	}
	gl_list_set_at(l->cellData, cnum, img);
}

/*
 * bta_get_base()
 */
bta_cell_t *bta_get_base(bta_t *bta)
{

	if (bta->base == NULL)
		return bta_cell_get(bta, 0, 0);

	return bta->base;
}

/*
 * bta_get_base()
 */
uint32_t bta_getNumCells(bta_t *bta, uint32_t lnum)
{
	assert(bta != NULL);

	bta_loop_t *btl = (bta_loop_t *)gl_list_get_at(bta->loopData, lnum);

	return gl_list_size(btl->cellData);
}

/*
 * bta_getNumLoops()
 */
uint32_t bta_getNumLoops(bta_t *bta)
{
	assert(bta != NULL);

	return gl_list_size(bta->loopData);
}

/*
 * bta_loop_get()
 */
bta_loop_t *bta_loop_get(bta_t *bta, uint32_t lnum)
{
	assert(bta != NULL);

	return (bta_loop_t *)gl_list_get_at(bta->loopData, lnum);
}
