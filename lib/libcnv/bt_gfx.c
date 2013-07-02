#include <stdint.h>
#include <bt_string.h>

#define getBit(x, shift) (((x) >> shift) & 0x01)
#define getHiPixel(b, l, ind, s) ((getBit(b[ind+(l*3)], s) << 7) | (getBit(b[ind+(l*2)], s) << 6) | (getBit(b[ind+l], s) << 5) | (getBit(b[ind], s) << 4))
#define getLoPixel(b, l, ind, s) ((getBit(b[ind+(l*3)], s) << 3) | (getBit(b[ind+(l*2)], s) << 2) | (getBit(b[ind+l], s) << 1) | (getBit(b[ind], s) << 0))

btstring_t *gfx_vga2gfx(btstring_t *in)
{
  btstring_t *rval;
  uint32_t i;
  uint32_t size;

  rval = bts_new(in->size);

  size = in->size / 4;

  for (i = 0; i < size; i++) {
    rval->buf[i*4] =     getHiPixel(in->buf, size, i, 7) | getLoPixel(in->buf, size, i, 6);
    rval->buf[(i*4)+1] = getHiPixel(in->buf, size, i, 5) | getLoPixel(in->buf, size, i, 4);
    rval->buf[(i*4)+2] = getHiPixel(in->buf, size, i, 3) | getLoPixel(in->buf, size, i, 2);
    rval->buf[(i*4)+3] = getHiPixel(in->buf, size, i, 1) | getLoPixel(in->buf, size, i, 0);
  }

  bts_free(in);

  return rval;
}

/*
btstring_t *gfx_4bitTo8bit(btstring_t *in)
{
  btstring_t *rval;
  uint32_t i;
  uint8_t *buf;

  rval = bts_new(in->size * 2);
  buf = rval->buf;

  for (i = 0; i < in->size; i++) {
    *buf++ = in->buf[i] >> 4;
    *buf++ = in->buf[i] & 0x0f;
  }

  bts_free(in);

  return rval;
}
*/
