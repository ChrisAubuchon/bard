#include <btlib.h>
#include <png.h>

#define DEBUG 1
#include <debug.h>

char 	*program_name = "btcs";

static uint8_t	copyBuffer[0x1000];

static void		initCopyBuffer(void);
static btstring_t	*unpackFile(uint8_t *inFile);
static void	 	picToPNG(uint8_t *inPicName, uint8_t *inPacName,
				 uint8_t *inPngName);
static void 		rix_toPNG(btstring_t *inBuffer, 
				  btstring_t *palBuffer, uint8_t *inName);


static void initCopyBuffer(void)
{
	uint32_t	i;

	for (i = 0; i < 4078; i++)
		copyBuffer[i] = 0x20;
	for (i = 4078; i < 0x1000; i++)
		copyBuffer[i] = 0;
}

static void picToPNG(uint8_t *inPicName, uint8_t *inPacName, uint8_t *inPngName)
{
	btstring_t	*picBuffer;
	btstring_t	*pacBuffer;

	picBuffer = unpackFile(inPicName);
	pacBuffer = unpackFile(inPacName);

	rix_toPNG(picBuffer, pacBuffer, inPngName);

	bts_free(picBuffer);
	bts_free(pacBuffer);

	return;
}

static btstring_t *unpackFile(uint8_t *inFile)
{
	uint32_t	i;
	uint32_t	copyIndex	= 0xfee;
	uint32_t	rvalIndex	= 0;
	uint32_t	readIndex	= 0;
	uint32_t	size		= 0;
	btstring_t	*rval;
	btstring_t	*readBuffer;
	uint16_t	control		= 0;
	uint16_t	offset;
	uint16_t	count;

	initCopyBuffer();

	readBuffer = readFile(inFile);
	dump_btstring(bts_strcpy("out.read"), readBuffer, 0);

	rval = bts_new(1024*128);

	while (1) {
		control >>= 1;
		if (!(control & 0x100)) {
			if (readIndex == readBuffer->size) 
				break;
			control = readBuffer->buf[readIndex++] | 0xff00;
		}

		if (!(control & 1)) {
			if (readIndex == readBuffer->size)
				break;

			offset = readBuffer->buf[readIndex++];
			if (readIndex == readBuffer->size)
				break;

			offset |= (readBuffer->buf[readIndex] & 0xf0) << 4;
			count = (readBuffer->buf[readIndex++] & 0x0f) + 3;
			size += count;

			for (i = 0; i < count; i++) {
				offset &= 0xfff;
				rval->buf[rvalIndex++] = copyBuffer[offset];
				copyBuffer[copyIndex++] = copyBuffer[offset++];
				copyIndex &= 0xfff;
			}
		} else {
			if (readIndex == readBuffer->size) 
				break;

			rval->buf[rvalIndex++] = readBuffer->buf[readIndex];
			size++;
			copyBuffer[copyIndex++] = readBuffer->buf[readIndex++];
			copyIndex &= 0xfff;
		}
	}

	bts_free(readBuffer);

	return bts_resize(rval, size);
}

static void rix_toPNG(btstring_t *inBuffer, btstring_t *palBuffer, uint8_t *inName)
{
	uint16_t	width;
	uint16_t	height;
	uint8_t		*palette;
	uint8_t		*img;
	uint32_t	i;

	FILE		*outfp;
	png_structp	png_ptr;
	png_infop	info_ptr;
	png_color	png_palette[256];

	width = str_read16le(&inBuffer->buf[4]);
	height = str_read16le(&inBuffer->buf[6]);

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

	outfp = fopen(inName, "wb");
	if (outfp == NULL) {
		printf("Error opening %s\n", inName);
		return;
	}

	png_init_io(png_ptr, outfp);
	png_set_IHDR(png_ptr, info_ptr, width, height, 8, \
		PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	/*palette = &inBuffer->buf[10];*/
	palette = palBuffer->buf;
	for (i = 0; i < 256; i++) {
		png_palette[i].red = *palette++ << 2;
		png_palette[i].green = *palette++ << 2;
		png_palette[i].blue = *palette++ << 2;
	}
	png_set_PLTE(png_ptr, info_ptr, png_palette, 256);

	png_write_info(png_ptr, info_ptr);

	for (i = 0; i < height; i++) {
		png_write_row(png_ptr, &inBuffer->buf[0x30a+(i * width)]);
	}

	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);

	fclose(outfp);
}


int main(void)
{
	FILE 		*fp;
	uint32_t	i;
	uint32_t	l_fileLength;
	btstring_t	*readBuffer;
	btstring_t	*outBuffer;
	btstring_t	*palBuffer;

	uint16_t	width;
	uint16_t	height;
	uint16_t	colors;


	picToPNG("BTSCREEN.PIC", "BTSCREEN.PAC", "btscreen.png");
}
