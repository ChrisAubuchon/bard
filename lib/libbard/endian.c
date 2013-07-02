/*
 * endian.c - Endian operations
 */
#include <btlib.h>

/*
 * str_read16be and read16le
 * Read two byte values in big-endian and little-endian from
 * a string
 */
uint16_t str_read16be(uint8_t *buf)
{
	uint16_t v = *((uint16_t *)buf);

	return cnv16be(v);
/*  return *((uint16_t *)buf);*/
}

uint16_t str_read16le(uint8_t *buf)
{
	uint16_t v = *((uint16_t *)buf);

	return cnv16le(v);
/*  return (uint16_t)((uint8_t)buf[0] | ((uint8_t)buf[1] << 8));*/
}

/*
 * str_read32be and str_read32le
 * Read four-byte values in big-endian and little-endian from
 * a string
 */
uint32_t str_read32be(uint8_t *buf)
{
	uint32_t v = *((uint32_t *)buf);

	return cnv32be(v);
/*  return *((uint32_t *)buf);*/
}

uint32_t str_read32le(uint8_t *buf)
{
	uint32_t v = *((uint32_t *)buf);

	return cnv32le(v);
/*  return (uint32_t)((uint8_t)buf[0] | ((uint8_t)buf[1] << 8) | ((uint8_t)buf[2] << 16) | ((uint8_t)buf[3] << 24));*/
}

/*
 * file_read16be and file_read16le
 * Read two-byte values from a file descriptor
 */
uint16_t file_read16be(int fd)
{
  uint16_t rval;

  xread(fd, &rval, sizeof(uint16_t));

  return cnv32be(rval);
}

uint16_t fp_read16be(FILE *fp)
{
	uint16_t rval;

	xfread(&rval, sizeof(uint16_t), 1, fp);

	return cnv16be(rval);
}

uint16_t file_read16le(int fd)
{
  uint16_t rval;

  xread(fd, &rval, sizeof(uint16_t));

  return cnv16le(rval);
}

uint16_t fp_read16le(FILE *fp)
{
	uint16_t rval;

	xfread(&rval, sizeof(uint16_t), 1, fp);

	return cnv16le(rval);
}

/*
 * file_read32be and file_read32le
 * Read four-byte values from a file descriptor
 */
uint32_t file_read32be(int fd)
{
  uint32_t rval;

  xread(fd, &rval, sizeof(uint32_t));

  return cnv32be(rval);
}

uint16_t fp_read32be(FILE *fp)
{
	uint32_t rval;

	xfread(&rval, sizeof(uint32_t), 1, fp);

	return cnv32be(rval);
}

uint32_t file_read32le(int fd)
{
  uint32_t rval;

  xread(fd, &rval, sizeof(uint32_t));

  return cnv32le(rval);
}

uint16_t fp_read32le(FILE *fp)
{
	uint32_t rval;

	xfread(&rval, sizeof(uint32_t), 1, fp);

	return cnv32le(rval);
}


void file_write16le(int fd, uint16_t val)
{
	uint16_t v;

	v = cnv16le(val);

	xwrite(fd, &v, sizeof(uint16_t));
}

void file_write32le(int fd, uint32_t val)
{
	uint32_t v;

	v = cnv32le(val);

	xwrite(fd, &v, sizeof(uint32_t));
}

void fp_write16le(FILE *fp, uint16_t val)
{
	uint16_t v;

	v = cnv16le(val);

	xfwrite(&v, sizeof(uint16_t), 1, fp);
}

void fp_write32le(FILE *fp, uint32_t val)
{
	uint32_t v;

	v = cnv32le(val);

	xfwrite(&v, sizeof(uint32_t), 1, fp);
}

