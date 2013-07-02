#ifndef _FILEIO_H
#define _FILEIO_H

#include <sys/file.h>
#include <stdlib.h>
#include <stdint.h>
#include <bt_string.h>

int xopen(char *fname, int flags, ...);
off_t xlseek(int fd, off_t offset, int whence);
ssize_t xread(int fd, void *buf, size_t nbytes);
ssize_t xwrite(int fd, void *buf, size_t nbytes);

void moveToIndex16(int fd, int index, int endian);
void moveToIndex32(int fd, int index, int endian);
btstring_t *readFile(uint8_t *fname);

void xmkdir(btstring_t *path);

#endif
