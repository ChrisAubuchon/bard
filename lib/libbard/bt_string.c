/*
 * bt_string.c - BT Strings 
 */
#include <btlib.h>
#include <stdarg.h>

btstring_t *bts_new(uint32_t size)
{
	btstring_t *rstr;

	rstr = (btstring_t *) xzalloc(sizeof(btstring_t));
	if (size) {
		rstr->buf = (uint8_t *) xzalloc(size);
		memset(rstr->buf, 0, size);
		rstr->size = size;
	}

	return rstr;
}

void bts_free(const void *vinstr)
{
	btstring_t *instr = (btstring_t *) vinstr;

	if (instr == NULL)
		return;

	free(instr->buf);
	free(instr);
	instr = NULL;
}

btstring_t *bts_copy(btstring_t *in)
{
	btstring_t *rval;
	uint8_t *src, *dest;
	uint32_t i;

	rval = bts_new(in->size);
	rval->size = in->size;

	src = in->buf;
	dest = rval->buf;

	for (i = 0; i < in->size; i++)
		*dest++ = *src++;

	return rval;
}

btstring_t *bts_ncopy(btstring_t *in, uint32_t len, uint32_t offset)
{
	btstring_t *rval;
	uint8_t *src, *dest;
	uint32_t i;

	rval = bts_new(len);

	src = &in->buf[offset];
	dest = rval->buf;

	for (i = 0; i < len; i++)
		*dest++ = *src++;

	return rval;
}

btstring_t *bts_strncpy(const uint8_t *str, uint32_t len)
{
	uint32_t i;
	btstring_t *rval;

	rval = bts_new(len + 1);
	for (i = 0; i < len; i++)
		rval->buf[i] = str[i];
	rval->buf[len] = '\0';

	return rval;
}

btstring_t *bts_sprintf(uint8_t *format, ...)
{
	btstring_t *rval;
	uint32_t len;
	va_list fmtargs;

	va_start(fmtargs, format);
	len = vsnprintf(NULL, 0, format, fmtargs);
	va_end(fmtargs);

	rval = bts_new(len + 1);
	va_start(fmtargs, format);
	vsnprintf(rval->buf, rval->size, format, fmtargs);
	va_end(fmtargs);

	return rval;
}

btstring_t *bts_strcpy(const uint8_t *str)
{
	btstring_t *rval;

	rval = bts_strncpy(str, strlen(str));

	return rval;
}

btstring_t *bts_strncat(btstring_t *dest, const uint8_t *str, uint32_t len)
{
	dest = bts_resize(dest, dest->size + len);
	strncat(dest->buf, str, len);

	return dest;
}

btstring_t *bts_strcat(btstring_t *dest, const uint8_t *str)
{
	return bts_strncat(dest, str, strlen(str));
}

btstring_t *bts_resize(btstring_t *in, uint32_t size)
{
	btstring_t *rval;
	uint32_t i;

	rval = bts_new(size);
	if (size > in->size)
		size = in->size;
	for (i = 0; i < size; i++)
		rval->buf[i] = in->buf[i];

	bts_free(in);

	return rval;
}

void bts_memset(btstring_t *t, int c)
{
	memset(t->buf, c, t->size);
}

size_t bts_hashcode(btstring_t *in)
{
	size_t 		hash = 5381;
	uint32_t	i;

	for (i = 0; i < in->size; i++)
		hash = ((hash << 5) + hash) + in->buf[i];

	return hash;
}
