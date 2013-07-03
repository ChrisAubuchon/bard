#include <dehuf.h>
#include <stdio.h>
#include <fileio.h>
#include <fpio.h>

static hufBitTree_t *newNode(void);
static uint8_t getNextBit(huffile_t *huf);
static uint8_t extractByte(huffile_t *huf);
static void free_bitTree(hufBitTree_t *curNode);
static void expandDecompTree(huffile_t *huf, hufBitTree_t *curNode);

/*
 * expandDecompTree()
 * A recursive function to decode the bits of a bard1 graphics
 * file into a CGA/EGA memory image.
 */
static void expandDecompTree(huffile_t *huf, hufBitTree_t *curNode) {
  if (getNextBit(huf) == 0) {
    curNode->left  = newNode();
    curNode->right = newNode();

    expandDecompTree(huf, curNode->left);
    getNextBit(huf);
    expandDecompTree(huf, curNode->right);
  } else {
    curNode->value = extractByte(huf);
  }
}


/*
 * newNode()
 * Allocate and initialize a new tree node
 */
static hufBitTree_t *newNode(void) {
  hufBitTree_t *new;

  new = (hufBitTree_t *)xzalloc(sizeof(hufBitTree_t));
  new->left = NULL;
  new->right = NULL;
  new->value = 0;

  return new;
}

/*
 * getNextBit()
 * Return the value of the next bit in the bit stream.
 * Read more data from the input file if necessary
 */
static uint8_t getNextBit(huffile_t *huf) {
  uint8_t curByte;

  /* If the bitmask is 0 we need to get a new byte */
  if (huf->mask == 0) {
    /* 
     * If we're at the end of the buffer, read in BUFSIZE
     * more bytes and reset the buffer index. Otherwise increment
     * the buffer index.
     */
    if (huf->bufIndex == (BUFSIZE - 1)) {
	fread(huf->buffer, sizeof(uint8_t), BUFSIZE, huf->fp);
      huf->bufIndex = 0;
    } else {
      huf->bufIndex++;
    }

    /* Reset the bit mask */
    huf->mask = 0x80;
  }

  /* Extract the current bit */
  curByte = huf->buffer[huf->bufIndex];
  curByte &= huf->mask;

  /* Shift the bitmask to the right to get the next bit next time through */
  huf->mask = huf->mask >> 1;
  return (curByte);
}

/*
 * extractByte()
 * Extract the next 8 bits into a CGA byte
 */
static uint8_t extractByte(huffile_t *huf) {
  uint8_t bits;
  uint8_t rbyte = 0;

  /* 
   * If the next bit in the bitstream is not set, set the
   * low bit of the return byte
   */
  for (bits = 0; bits < 8; bits++) {
    rbyte = rbyte << 1;
    if (getNextBit(huf) != 0)
      rbyte |= 1;
  }

  return rbyte;
}

btstring_t *dehuf(huffile_t *huf, uint16_t maxsize)
{
  btstring_t *rval;
  hufBitTree_t *cur;
  int bytesWritten = 0;
  int offset = 0;
  int16_t cf;

  if (huf->header != 0) {
    if ((uint16_t)(maxsize - huf->maxSize) >= 0) {
      cf = 0;
    } else {
      cf = 1;
    }
    if ((uint16_t)(0 - huf->header - cf) >=0) {
      huf->size = maxsize;
	}

  } else {
    huf->size = maxsize;
  }

  rval = bts_new((huf->size * sizeof(uint8_t)) + 1);

  while (bytesWritten != huf->size) {
    cur = huf->treeRoot;

    while (cur->left != NULL) {
      if (getNextBit(huf) == 0) 
	cur = cur->left;
      else
	cur = cur->right;
    }
    rval->buf[offset++] = cur->value;
    bytesWritten++;
  }

  rval->size = bytesWritten;

  return rval;
}

/*
 * dehuf_init()
 * Initialize the decompression of one of the bard graphics
 * files. This function assumes that infd is a valid file and
 * that the file position is at the head of the file.
 */
/*huffile_t *dehuf_init(int infd)*/
huffile_t *dehuf_init(FILE *infp)
{ 
  huffile_t *huf;

  /* Allocate a new huffile_t stucture */
  huf = (huffile_t *)xzalloc(sizeof(huffile_t));

	huf->fp = infp;

  /* Read in the first 0x200 bytes as a buffer */
  xfread(huf->buffer, sizeof(uint8_t), BUFSIZE, huf->fp);

  huf->header = str_read16be(huf->buffer);
  huf->maxSize = str_read16be(huf->buffer + 2);

  huf->bufIndex = 8;
  huf->mask = 0x80;

  /* Create the root of the decompression tree */
  huf->treeRoot = (hufBitTree_t *)xzalloc(sizeof(hufBitTree_t));
  huf->treeRoot->left = NULL;
  huf->treeRoot->right = NULL;
  huf->treeRoot->value = 0;

  /* Expand the decompression tree */
  expandDecompTree(huf, huf->treeRoot);

  return huf;
}

/*
 * dehuf_free()
 * Free memory associated with a huffile_t
 */
void dehuf_free(huffile_t *huf)
{
  free_bitTree(huf->treeRoot);
  free(huf);
}

/*
 * free_bitTree()
 * Free memory associated with the bit tree
 */
static void free_bitTree(hufBitTree_t *curNode)
{
  if (curNode->left != NULL)
    free_bitTree(curNode->left);
  if (curNode->right != NULL)
    free_bitTree(curNode->right);

  free(curNode);
}

#if 0
btstring_t *dehufFile(uint8_t *fname, uint16_t size)
{
  huffile_t *huf;
  btstring_t *rval;
	FILE *fp;


	fp = xfopen(bts_strcpy(fname), "rb");
	huf = dehuf_init(fp);
  rval = dehuf(huf, size);
  dehuf_free(huf);
	fclose(fp);

  return rval;
}
#endif 

btstring_t *dehufFile(btstring_t *fname, uint16_t size)
{
	huffile_t	*huf;
	btstring_t	*rval;
	FILE		*fp;

	fp = xfopen(fname, "rb");
	huf = dehuf_init(fp);
	rval = dehuf(huf, size);
	dehuf_free(huf);
	fclose(fp);
	bts_free(fname);

	return rval;
}
