#include <stdio.h>
#include <bt_zlib.h>

btstring_t *zlib_compress(btstring_t * fb)
{
	btstring_t *rb;
	uLongf size;
	int rval;

	rb = (btstring_t *) xzalloc(sizeof(btstring_t));

	size = compressBound(fb->size);
	rb->size = size;
	rb->buf = (uint8_t *) xzalloc(size);

	rval = compress(rb->buf, &size, fb->buf, fb->size);
	if (rval != Z_OK) {
		printf("Error compressing: %d\n", rval);
	}

	rb = bts_resize(rb, size);

	bts_free(fb);

	return rb;
}

btstring_t *zlib_uncompress(btstring_t *fb, uLongf size)
{
	btstring_t *rb;

	rb = bts_new(size);

	if (uncompress(rb->buf, &size, fb->buf, fb->size) != Z_OK)  {
		printf("Error uncompressing\n");
		exit(1);
	}

	bts_free(fb);

	return rb;
}
