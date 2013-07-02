#ifndef _BT_ZLIB_H
#define _BT_ZLIB_H

#include <bt_string.h>
#include <zlib.h>

btstring_t *zlib_compress(btstring_t *fb);
btstring_t *zlib_uncompress(btstring_t *fb, uLongf size);

#endif
