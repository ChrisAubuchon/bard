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

bta_color_t egapal[16] = {
 {   0,   0,   0, 255 }, /*0*/
 {   0,   0, 170, 255 }, /*1*/
 {   0, 170,   0, 255 }, /*2*/
 {   0, 170, 170, 255 }, /*3*/
 { 170,   0,   0, 255 }, /*4*/
 { 170,   0, 170, 255 }, /*5*/
 { 170,  85,   0, 255 }, /*6*/
 { 170, 170, 170, 255 }, /*7*/
 {  85,  85,  85, 255 }, /*8*/
 {  85,  85, 255, 255 }, /*9*/
 {  85, 255,  85, 255 }, /*a*/
 {  85, 255, 255, 255 }, /*b*/
 { 255,  85,  85, 255 }, /*c*/
 { 255,  85, 255, 255 }, /*d*/
 { 255, 255,  85, 255 }, /*e*/
 { 255, 255, 255, 255 }  /*f*/
};

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

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

/*
 * bta_cell_scale()
 *
 * Scale from 320x200 to 640x400
 */
void bta_cell_scale(bta_cell_t *in)
{
	uint16_t	x, y;
	bta_cell_t	*rval;
	uint8_t		*srcbuf, *dstbuf, *tmpbuf;

	btstring_t	*newgfx;
	uint16_t	newX, newY;
	uint16_t	newWidth, newHeight;
	uint16_t	newDelay;


	newX		= in->x * 2;
	newY		= in->y * 2;
	newWidth	= in->width * 2;
	newHeight	= in->height * 2;
	newDelay	= in->delay * 60;

	newgfx = bts_new(newWidth * newHeight);

	srcbuf = in->gfx->buf;
	dstbuf = newgfx->buf;

	for (y = 0; y < in->height; y++) {
		tmpbuf = dstbuf;

		for (x = 0; x < in->width; x++) {
			*dstbuf++ = *srcbuf;
			*dstbuf++ = *srcbuf++;
		}

		memcpy(dstbuf, tmpbuf, newWidth);
		dstbuf += newWidth;
	}

	bts_free(in->gfx);
	in->x		= newX;
	in->y		= newY;
	in->width	= newWidth;
	in->height	= newHeight;
	in->delay	= newDelay;
	in->gfx		= newgfx;
/*	printf("Here\n");*/
}

/*
 * bta_trim()
 *
 * Trim bytes from the left side and right side of the image
 */
void bta_trim(bta_cell_t *in, uint16_t left, uint16_t right)
{
	uint16_t	x,y;
	uint32_t	soffset;
	uint32_t	doffset;

	btstring_t	*newgfx;
	uint16_t	newWidth;

	if ((left == 0) && (right == 0))
		return;

	if (in->width - left - right <= 0)
		return;

	newWidth = in->width - left - right;
	newgfx = bts_new(in->height * newWidth);

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
			
			newgfx->buf[doffset++] = in->gfx->buf[soffset++];
		}
	}

	bts_free(in->gfx);
	in->width = newWidth;
	in->gfx = newgfx;
}

/*
 * bta_cell_4bitTo8bit()
 *
 * Unpack 4 bit data to 8 bit
 */
void bta_cell_4bitTo8bit(bta_cell_t *in)
{
	uint8_t		*tobuf, *frombuf;
	uint32_t	i;

	uint16_t	newWidth;
	uint16_t	newX;
	btstring_t	*newgfx;

	newX		= in->x * 2;
	newWidth	= in->width * 2;
	newgfx		= bts_new(newWidth * in->height);

	tobuf = newgfx->buf;
	frombuf = in->gfx->buf;

	for (i = 0; i < in->gfx->size; i++) {
		*tobuf++ = *frombuf >> 4;
		*tobuf++ = *frombuf++ & 0x0f;
	}

	bts_free(in->gfx);
	in->width	= newWidth;
	in->x		= newX;
	in->gfx		= newgfx;
}


/*
 * bta_cell_toRGBA()
 *
 * Convert a palettized image to RGBA
 */
void bta_cell_toRGBA(bta_cell_t *in, bta_color_t *pal)
{
	btstring_t	*newgfx;
	uint8_t		*tobuf, *frombuf;
	uint32_t	i;

	newgfx = bts_new((in->width * in->height) * 4);

	tobuf	= newgfx->buf;
	frombuf = in->gfx->buf;

	for (i = 0; i < in->gfx->size; i++) {
		*tobuf++ = pal[*frombuf].red;
		*tobuf++ = pal[*frombuf].green;
		*tobuf++ = pal[*frombuf].blue;
		*tobuf++ = pal[*frombuf++].alpha;
	}

	bts_free(in->gfx);
	in->gfx = newgfx;
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
		PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, \
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_write_info(png_ptr, info_ptr);

	for (i = 0; i < in->height; i++)  
		png_write_row(png_ptr, &in->gfx->buf[(i * in->width) * 4]);

	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);

	fclose(outfp);
	bts_free(fname);
}

/*
 * bta_transparent_toPNG
 */
void bta_transparent_toPNG(bta_cell_t *in, btstring_t *fname)
{
	FILE		*outfp;
	png_structp	png_ptr;
	png_infop	info_ptr;
	uint32_t	i;
	int		trans = 0;

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
	png_set_tRNS(png_ptr, info_ptr, (png_bytep)&trans, 1, NULL);

	png_write_info(png_ptr, info_ptr);

	for (i = 0; i < in->height; i++)  
		png_write_row(png_ptr, &in->gfx->buf[i * in->width]);

	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);

	fclose(outfp);
	bts_free(fname);
}
