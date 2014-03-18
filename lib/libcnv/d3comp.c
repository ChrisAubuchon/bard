#include <dehuf.h>
#include <stdio.h>
#include <fpio.h>
#include <endian.h>

/*#define DEBUG 1*/
#include <debug.h>

/*
 * Global module variables
 */
static uint8_t	*dataBuf;
static uint16_t curFromOffset;
static uint16_t toOffset = 0;

static uint8_t workBuf[128];
static uint16_t workBufIndex;

static uint8_t transBuf[4155];
static uint16_t transBufIndex = 0;

static uint16_t d3cmpMem[2200];

static uint16_t dataWord;
static uint8_t dataShift;

static uint32_t dataHeader;
static uint32_t countMaybe;

#define writeWorkBuf(x, y) ((uint16_t *)workBuf)[(x)] = cnv16be((y))
#define readWorkBuf(x) workBuf[(x)]

/*
 * Function prototypes
 */
static void d3cmp_readData(void);
static void d3cmp_init(void);
static void d3cmp_doDecomp(void);
static void d3cmp_updateMemory(uint16_t val);
static uint16_t d3cmp_getNextWord(void);
static void d3cmp_updateDataWord(void);
static uint16_t d3cmp_readCopyOffset(void);
static void sub_d3cmp_outputToBuffer(void);

static btstring_t *outBuf;

static uint8_t byte_4ca3f[] = {
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

static uint8_t byte_4cb3f[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9,
	10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11,
	12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15,
	16, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 18, 19, 19, 19, 19,
	20, 20, 20, 20, 21, 21, 21, 21, 22, 22, 22, 22, 23, 23, 23, 23,
	24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31,
	32, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39,
	40, 40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47,
	48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63
};

static void d3cmp_init(void)
{
	int i;
	uint16_t tmp;

	for (i = 0; i < 2200; i++)
		d3cmpMem[i] = 0;

	for (i = 0; i < 314; i++) {
		d3cmpMem[i] = 1;
		d3cmpMem[i + 1569] = i + 627;
		d3cmpMem[i + 627 + 628] = i;
	}

	tmp = 0;
	for (i = 314; i <= 626; i++) {
		d3cmpMem[i] = d3cmpMem[tmp] + d3cmpMem[tmp + 1];
		d3cmpMem[i + 1569] = tmp;
		d3cmpMem[tmp + 628] = i;
		d3cmpMem[tmp + 629] = i;
		tmp += 2;
	}

	d3cmpMem[627] = 0xffff;
	d3cmpMem[1254] = 0;

/*
  for (int i = 0; i < 2200; i++)
    debug("d3cmpMem[%d] = %d\n", i, d3cmpMem[i]);
*/
}

static void d3cmp_updateMemory(uint16_t val)
{
	uint16_t index;
	uint16_t offset;
	uint16_t saved;
	uint16_t swap;
	uint16_t i;

	debug("Entering d3cmp_updateMemory()\n");

	debug("d3cmpMem[626] = %x\n", d3cmpMem[626]);
	if (d3cmpMem[626] == 0x8000) {
		printf("Calling sub_27980()\n");
		exit(0);
	}

	index = d3cmpMem[val + 1255];

	debug("val = %x\n", val);

      loc_276a1:
	debug("index = %x\n", index);

	d3cmpMem[index]++;
	offset = index + 1;

	if (d3cmpMem[offset] < d3cmpMem[index]) {
		while (d3cmpMem[index] > d3cmpMem[offset])
			offset++;
		offset--;

		swap = d3cmpMem[index];
		d3cmpMem[index] = d3cmpMem[offset];
		d3cmpMem[offset] = swap;

		i = d3cmpMem[index + 1569];
		d3cmpMem[i + 628] = offset;
		if (i < 627)
			d3cmpMem[i + 629] = offset;

		saved = d3cmpMem[offset + 1569];
		d3cmpMem[offset + 1569] = i;
		d3cmpMem[saved + 628] = index;
		if (saved < 627) {
			d3cmpMem[saved + 629] = index;
		}

		d3cmpMem[index + 1569] = saved;
	} else {
		offset = index;
	}

	index = d3cmpMem[offset + 628];
	if (index)
		goto loc_276a1;
}

static void d3cmp_updateDataWord(void)
{
	uint8_t ch;
	uint8_t shift = 8;

	do {
		if (workBufIndex & 0x80) {
			d3cmp_readData();
			workBufIndex = 0;
		}

		ch = readWorkBuf(workBufIndex);
		workBufIndex++;

		shift -= dataShift;

		dataWord |= (ch << shift);
		dataShift += 8;
	} while (shift & 8);
}

static uint16_t d3cmp_getNextWord(void)
{
	int i;
	uint8_t bit;
	uint16_t val;

	debug("Entering d3cmp_getNextWord()\n");

	val = d3cmpMem[2195];

      loc_27791:
	debug("val = %d\n", val);
	if (val < 627) {
		debug("dataShift = %d\n", dataShift);
		if (dataShift <= 8) {
			d3cmp_updateDataWord();
		}

		bit = 0;
		if (dataWord & 0x8000)
			bit = 1;
		dataWord <<= 1;
		debug("dataWord = %x\n", dataWord);
		dataShift--;

		val += bit;
		debug("d3cmpMem[%d + 0xc42] = %d\n", val, d3cmpMem[val + 1569]);
		val = d3cmpMem[val + 1569];
		goto loc_27791;
	}

	val -= 627;
	d3cmp_updateMemory(val);

	return (val);
	debug("returning from d3cmp_getNextWord()\n");
}

void sub_d3cmp_outputToBuffer(void)
{
	int i;

	debug("Called sub_d3cmp_outputToBuffer\n");

	for (i = 0; i < transBufIndex; i++)
		outBuf->buf[toOffset + i] = transBuf[i];

	toOffset += transBufIndex;
}

static uint16_t d3cmp_readCopyOffset(void)
{
	uint8_t index;
	uint16_t rval;
	uint8_t savedBit;
	uint16_t count;

	debug("Entering d3cmp_readCopyOffset()\n");

	debug("dataShift = %d\n", dataShift);
	if (dataShift <= 8)
		d3cmp_updateDataWord();

	debug("dataWord = %x\n", dataWord);

	dataShift -= 8;
	index = dataWord >> 8;
	dataWord <<= 8;

	debug("dataWord = %x\n", dataWord);
	debug("index = %x\n", index);
	rval = byte_4cb3f[index] << 6;
	debug("rval = %x\n", rval);

	count = byte_4ca3f[index] - 2;

	if (count != 0) {
	      loc_27812:
		index <<= 1;
		debug("dataShift = %d\n", dataShift);
		if (dataShift <= 8)
			d3cmp_updateDataWord();
		savedBit = 0;
		if (dataWord & 0x8000)
			savedBit++;
		debug("dataWord = %x\n", dataWord);
		dataWord <<= 1;
		dataShift--;

		index += savedBit;
		count--;
		if (count != 0)
			goto loc_27812;
	}

	return (rval | (index & 0x3f));
}

static void d3cmp_doDecomp(void)
{
	uint16_t data;
	uint16_t _d3cmp_base;
	uint16_t _d3cmp_offset;

	debug("Entering d3cmp_doDecomp()\n");

	if (dataHeader) {
		d3cmp_init();
		transBufIndex = 0;
		countMaybe = 0;

		while (countMaybe < dataHeader) {
			debug("dataHeader = %d, count = %d\n", dataHeader, countMaybe);

			data = d3cmp_getNextWord();
			if (data < 256) {
				debug("data = %x\n", data);
				transBuf[transBufIndex++] = data;
				if (transBufIndex & 4096) {
					sub_d3cmp_outputToBuffer();
					transBufIndex = 0;
				}
				countMaybe++;
			} else {
				data -= 253;
				_d3cmp_base = transBufIndex - d3cmp_readCopyOffset() - 1;
				_d3cmp_offset = 0;
				while (_d3cmp_offset < data) {
					transBuf[transBufIndex++] = transBuf[(_d3cmp_base + _d3cmp_offset) & 0x0fff];
					if (transBufIndex & 0x1000) {
						sub_d3cmp_outputToBuffer();
						transBufIndex = 0;
					}
					countMaybe++;
					_d3cmp_offset++;
				}
			}
		}
		sub_d3cmp_outputToBuffer();
	}
}

static void d3cmp_readData(void)
{
	int i;

	debug("curFromOffset = %d\n", curFromOffset);

	for (i = 0; i < 64; i++)
		writeWorkBuf(i, str_read16be(&dataBuf[curFromOffset + (i * 2)]));

	curFromOffset += 128;

	for (i = 0; i < 128; i++)
		debug("workBuf[%d] = %0x\n", i, readWorkBuf(i));

	workBufIndex = 0;
}

btstring_t *d3comp(uint8_t *buf, uint32_t size)
{
	btstring_t	*rval;
	int		i;

	curFromOffset = 0;
	toOffset = 0;
	transBufIndex = 0;
	dataShift = 0;
	dataWord = 0;

	outBuf = bts_new(size);

	dataBuf = buf;

	dataHeader = str_read32le(&dataBuf[curFromOffset]);

	curFromOffset += 4;

	for (i = 0; i < 4155; i++)
		transBuf[i] = ' ';

	dataWord = 0;
	d3cmp_readData();
	d3cmp_doDecomp();

	rval = bts_strdup(outBuf);
	bts_resize(rval, toOffset);

	bts_free(outBuf);

	return rval;
}

btstring_t *d3compFile(btstring_t *fname, uint32_t size)
{
	btstring_t *data;
	btstring_t *rval;

	data = fp_readFile(fname);
	rval = d3comp(data->buf, size);

	bts_free(data);

	return rval;
}
