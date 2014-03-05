#ifndef _DEHUF_H
#define _DEHUF_H

#include <stdio.h>
#include <stdint.h>
#include <bt_string.h>

#define MAX_TREE        0x300
#define BUFSIZE         0x200

typedef struct bitTree {
  struct bitTree *left;
  struct bitTree *right;
  uint8_t value;
} hufBitTree_t;

typedef struct {
	FILE *fp;
  int           fd;             /* File descriptor              */
  uint16_t      header;         /* First two bytes of file. Not sure of use     */
  uint16_t      maxSize;        /* Second two bytes of file. Sets max # of bytes        */
  uint16_t      size;
  uint8_t       mask;           /* Decompression bitmask        */
  uint32_t      bufIndex;
  uint8_t       buffer[BUFSIZE];        /* */
  hufBitTree_t     *treeRoot;
} huffile_t;

/*huffile_t *dehuf_init(int infd);*/
huffile_t *dehuf_init(FILE *fp);
btstring_t *dehuf(huffile_t *huf, uint16_t maxsize);
void dehuf_free(huffile_t *huf);
#if 0
btstring_t *dehufFile(uint8_t *fname, uint16_t size);
#endif
btstring_t *dehufFile(btstring_t *fname, uint16_t size);

btstring_t *d3comp(uint8_t *buf, uint32_t size);
btstring_t *d3compFile(btstring_t *fname, uint32_t size);

#endif

