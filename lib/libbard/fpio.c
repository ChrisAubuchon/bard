#include <fpio.h>

/*
 * xfopen()
 */
FILE *xfopen(btstring_t *fname, uint8_t *mode)
{
	FILE *fp;

	fp = fopen(fname->buf, mode);
	if (fp == NULL) {
		perror("xfopen()");
		fprintf(stderr, "file: %s", fname->buf);
		exit(1);
	}

	return fp;
}

/*
 * xfseek()
 */
int xfseek(FILE *fp, long int offset, int origin)
{
	int rval;

	rval = fseek(fp, offset, origin);
	if (rval) {
		perror("xfseek()");
		exit(1);
	}

	return ftell(fp);
}

/*
 * xfread()
 */
size_t xfread(void *ptr, size_t size, size_t count, FILE *fp)
{
	size_t c;

	c = fread(ptr, size, count, fp);
/*
	if (c != count) {
		perror("xfread()");
		exit(1);
	}
*/

	return c;
}

/*
 * xfwrite()
 */
size_t xfwrite(void *ptr, size_t size, size_t count, FILE *fp)
{
	size_t c;

	c = fwrite(ptr, size, count, fp);
	if (c != count) {
		perror("xfwrite()");
		exit(1);
	}

	return c;
}

/*
 * fp_moveToIndex16()
 */
void fp_moveToIndex16(FILE *fp, int index, int endian)
{
	uint16_t dest;

	xfseek(fp, index * sizeof(uint16_t), SEEK_SET);
	if (endian)
		dest = fp_read16be(fp);
	else
		dest = fp_read16le(fp);
	xfseek(fp, dest, SEEK_SET);
}

/*
 * fp_moveToIndex32()
 */
void fp_moveToIndex32(FILE *fp, int index, int endian)
{
	uint32_t dest;

	xfseek(fp, index * sizeof(uint32_t), SEEK_SET);
	if (endian)
		dest = fp_read32be(fp);
	else
		dest = fp_read32le(fp);

	xfseek(fp, dest, SEEK_SET);
}

/*
 * fp_readFile()
 */
btstring_t *fp_readFile(btstring_t *fname)
{
	FILE		*fp;
	int		size;
	btstring_t	*rval;

	fp = xfopen(fname, "rb");
	size = xfseek(fp, 0, SEEK_END);
	xfseek(fp, 0, SEEK_SET);

	rval = bts_new(size);
	xfread(rval->buf, 1, size, fp);

	fclose(fp);
	bts_free(fname);

	return rval;
}
