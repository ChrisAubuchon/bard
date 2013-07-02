/*
 * fileio.c - Error checking file operations
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/file.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fileio.h>
#include <assert.h>

/*
 * xopen()
 * Error checking open
 */
int xopen(char *fname, int flags, ...)
{
  int rfd;
  int mode;

#ifdef O_BINARY
	flags |= O_BINARY;
#endif

  if (flags & O_CREAT) {
    va_list arg;
    va_start(arg, flags);
    mode = va_arg(arg, int);
    va_end(arg);

    rfd = open(fname, flags, mode);
  } else {
    rfd = open(fname, flags);
  }

  if (rfd < 0) {
    perror("xopen()");
    exit(1);
  }

  return rfd;
}

/*
 * xlseek()
 * Error checking lseek
 */
off_t xlseek(int fd, off_t offset, int whence)
{
  off_t rval;

  rval = lseek(fd, offset, whence);
  if (rval < 0) {
    perror("xlseek()");
    exit(1);
  }

  return rval;
} 

/*
 * xread()
 * Error check read
 */
ssize_t xread(int fd, void *buf, size_t nbytes)
{
  ssize_t rval;

  rval = read(fd, buf, nbytes);
  if (rval < 0) {
    perror("xread()");
    exit(1);
  }

  return rval;
}

/*
 * xwrite()
 * Error checking write
 */
ssize_t xwrite(int fd, void *buf, size_t nbytes)
{
  ssize_t rval;

  rval = write(fd, buf, nbytes);
  if (rval != nbytes) {
    perror("xwrite()");
    exit(1);
  }
}

btstring_t *readFile(uint8_t *fname)
{
  btstring_t *rval;
  ssize_t size;
  int fd;

  fd = xopen(fname, O_RDONLY);
  size = xlseek(fd, 0, SEEK_END);
  xlseek(fd, 0, SEEK_SET);
  rval = bts_new(size);
  xread(fd, rval->buf, rval->size);
  close(fd);

  /*rval->buf[rval->size] = '\0';*/

  return rval;
}


/*
 * moveToIndex16()
 * Read in a little-endian 16 bit number from a table
 * and move the file pointer to that location
 */
void moveToIndex16(int fd, int index, int endian)
{
  uint16_t dest;

  xlseek(fd, index * sizeof(uint16_t), SEEK_SET);
  if (endian)
    dest = file_read16be(fd);
  else
    dest = file_read16le(fd);
  xlseek(fd, dest, SEEK_SET);
}

/*
 * moveToIndex32()
 * Read in a little-endian 32 bit number from a table
 * and move the file pointer to that location
 */
void moveToIndex32(int fd, int index, int endian)
{
  uint32_t dest;

  xlseek(fd, index * sizeof(uint32_t), SEEK_SET);
  if (endian)
    dest = file_read32be(fd);
  else
    dest = file_read32le(fd);
  xlseek(fd, dest, SEEK_SET);
}


/*
 * xmkdir()
 * Make a directory
 */
void xmkdir(btstring_t *path)
{
	int rval;

#ifdef __MINGW32__
	rval = mkdir(path->buf);
#else
	rval = mkdir(path->buf, 0755);
#endif
	if (rval < 0) {
		if (errno != EEXIST)
			perror("xmkdir(): ");
	}

	bts_free(path);
}
