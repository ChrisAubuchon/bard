#include <stdio.h>
#include <cnv_gfx.h>
#include <bt_string.h>
#include <png.h>

#define DEBUG
#include <debug.h>

/********************************/
/*				*/
/* Private data			*/
/*				*/
/********************************/

static png_color egapalette[16] = {
 {   0,   0,   0 }, /*0*/
 {   0,   0, 170 }, /*1*/
 {   0, 170,   0 }, /*2*/
 {   0, 170, 170 }, /*3*/
 { 170,   0,   0 }, /*4*/
 { 170,   0, 170 }, /*5*/
 { 170,  85,   0 }, /*6*/
 { 170, 170, 170 }, /*7*/
 {  85,  85,  85 }, /*8*/
 {  85,  85, 255 }, /*9*/
 {  85, 255,  85 }, /*a*/
 {  85, 255, 255 }, /*b*/
 { 255,  85,  85 }, /*c*/
 { 255,  85, 255 }, /*d*/
 { 255, 255,  85 }, /*e*/
 { 255, 255, 255 }  /*f*/
};


/********************************/
/*				*/
/* Private function prototypes	*/
/*				*/
/********************************/

/********************************/
/*				*/
/* Private functions		*/
/*				*/
/********************************/


btstring_t *prepDataXML(btstring_t *in)
{
	return in;
}

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

bta_cell_t *bta_cell_scale(bta_cell_t *in)
{
	uint16_t x, y;
	bta_cell_t *rval;
	uint8_t *srcbuf, *dstbuf, *tmpbuf;

	rval = bta_cell_new(in->x * 2, in->y * 2, in->width * 2, in->height * 2, in->delay * 60, NULL);
	rval->gfx = bts_new(rval->width * rval->height);

	srcbuf = in->gfx->buf;
	dstbuf = rval->gfx->buf;

	for (y = 0; y < in->height; y++) {
		tmpbuf = dstbuf;

		for (x = 0; x < in->width; x++) {
			*dstbuf++ = *srcbuf;
			*dstbuf++ = *srcbuf++;
		}

		memcpy(dstbuf, tmpbuf, rval->width);
		dstbuf += rval->width;
	}
	bta_cell_free(in);

	return rval;
}

/*
 * bta_trim()
 *
 * Trim bytes from the left side and right side of the image
 */
bta_cell_t *bta_trim(bta_cell_t *in, uint16_t left, uint16_t right)
{
	bta_cell_t *rval;
	uint16_t x,y;
	uint32_t soffset;
	uint32_t doffset;

	if ((left == 0) && (right == 0))
		return in;

	if (in->width - left - right <= 0)
		return NULL;

	rval = bta_cell_new(in->x, in->y, in->width - left - right, \
				in->height, 0, NULL);
	rval->gfx = bts_new(rval->height * rval->width);

	soffset = 0;
	doffset = 0;
	for (y = 0; y < in->height; y++) {
		for (x = 0; x < in->width; x++) {
			if (x < left) {
				soffset++;
				continue;
			}

			if (x > (in->width - right - 1)) {
				soffset++;
				continue;
			}
			
			rval->gfx->buf[doffset++] = in->gfx->buf[soffset++];
		}
	}

	bta_cell_free(in);

	return rval;
}

bta_cell_t *bta_cell_4bitTo8bit(bta_cell_t *in)
{
	bta_cell_t *rval;
	uint8_t *tobuf, *frombuf;
	uint32_t i;

	rval = bta_cell_new(in->x * 2, in->y, in->width * 2, in->height, in->delay, NULL);
	rval->gfx = bts_new(rval->width * rval->height);

	tobuf = rval->gfx->buf;
	frombuf = in->gfx->buf;

	for (i = 0; i < in->gfx->size; i++) {
		*tobuf++ = *frombuf >> 4;
		*tobuf++ = *frombuf++ & 0x0f;
	}

	bta_cell_free(in);

	return rval;
}

void bta_toPNG(bta_cell_t *in, btstring_t *fname)
{
	FILE *outfp;
	png_structp png_ptr;
	png_infop info_ptr;
	uint32_t i;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		printf("Failed allocating png_ptr\n");
		return;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		printf("Failed allocating info_ptr\n");
		return;
	}

	outfp = fopen(fname->buf, "wb");
	if (outfp == NULL) {
		printf("Error opening %s\n", fname->buf);
		return;
	}

	png_init_io(png_ptr, outfp);
	png_set_IHDR(png_ptr, info_ptr, in->width, in->height, 8, \
		PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_NONE, \
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);


	png_set_PLTE(png_ptr, info_ptr, egapalette, 16);

	png_write_info(png_ptr, info_ptr);

	for (i = 0; i < in->height; i++)  
		png_write_row(png_ptr, &in->gfx->buf[i * in->width]);

	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);

	fclose(outfp);
	bts_free(fname);
}
