#ifndef _LIBBARD_FPIO_H
#define _LIBBARD_FPIO_H

#include <btlib.h>

FILE *xfopen(btstring_t *fname, uint8_t *mode);
int xfseek(FILE *fp, long int offset, int origin);
size_t xfread(void *ptr, size_t size, size_t count, FILE *fp);
size_t xfwrite(void *ptr, size_t size, size_t count, FILE *fp);

void fp_moveToIndex16(FILE *fp, int index, int endian);
void fp_moveToIndex32(FILE *fp, int index, int endian);

#endif
