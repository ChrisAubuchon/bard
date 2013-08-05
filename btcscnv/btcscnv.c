#include <btlib.h>

#define DEBUG 1
#include <debug.h>

char 	*program_name = "btcs";

static uint8_t	unkBuffer[0x1000];

static btstring_t *unpackFile(btstring_t *readBuffer);
static btstring_t *unpackFile(btstring_t *readBuffer)
{
	uint32_t	unkBufferSize;
	uint16_t	v18;
	uint32_t	index;
	uint32_t	i;
	uint32_t	rvalIndex	= 0;
	uint32_t	outputSize	= 0;
	btstring_t	*rval		= bts_new(65536);
	uint8_t		ch;
	uint16_t	ax,bx,cx,dx,si;

	unkBufferSize		= 0xfee;
	v18			= 0;
	index			= 0;

loc_26D24:
	if (index > readBuffer->size) {
		debug("Returning broken\n");
		return rval;
	}
	v18 >>= 1;
	if (!(v18 & 0x100)) {
		if (index == readBuffer->size) {
			return rval;
		}
		v18 = readBuffer->buf[index++] | 0xff00;
	}
	if (!(v18 & 1)) {
		if (index == readBuffer->size) {
			return rval;
		}

		ax = readBuffer->buf[index++];
		if (index == readBuffer->size) {
			return rval;
		}

		dx = ax & 0xff;
		ax = readBuffer->buf[index++] & 0xff;
		cx = (ax & 0xf0) << 4;
		dx |= cx;
		ax = (ax & 0x0f) + 2;
		outputSize += ax;
		outputSize++;
		cx = ax + 1;

		si = dx;
		for (i = 0; i < cx; i++) {
			si &= 0xfff;
			ch = unkBuffer[si++];
			rval->buf[rvalIndex++] = ch;
			unkBuffer[unkBufferSize++] = ch;
			unkBufferSize &= 0xfff;
		}
		goto loc_26D24;
	} else {
		if (index == readBuffer->size) {
			return rval;
		}

		ch = readBuffer->buf[index++];
		rval->buf[rvalIndex++] = ch;
		outputSize++;
		unkBuffer[unkBufferSize++] = ch;
		unkBufferSize &= 0xfff;
		goto loc_26D24;
	}
}


int main(void)
{
	FILE 		*fp;
	uint32_t	i;
	uint32_t	l_fileLength;
	btstring_t	*readBuffer;
	btstring_t	*outBuffer;

	uint32_t	v18;
	uint32_t	index;

	for (i = 0; i < 4078; i++)
		unkBuffer[i] = 0x20;

	/*fp = xfopen(bts_strcpy("BTSCREEN.PIC"), "rb");*/
	fp = xfopen(bts_strcpy("LOGO.PIC"), "rb");
	l_fileLength = xfseek(fp, 0, SEEK_END);
	xfseek(fp, 0, SEEK_SET);
	readBuffer = bts_new(l_fileLength);
	xfread(readBuffer->buf, l_fileLength, 1, fp);
	fclose(fp);

	outBuffer = unpackFile(readBuffer);
	dump_btstring(bts_strcpy("out.rix"), outBuffer, 0);
}
