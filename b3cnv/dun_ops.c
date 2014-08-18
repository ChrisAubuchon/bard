#include <b3lib.h>
#include <dun.h>

static btstring_t	*readData(uint8_t);
static btstring_t	*getLabel(int32_t, int32_t);

/****************************************/
/*					*/
/* Local functions			*/
/*					*/
/****************************************/

/*
 * readLevel()
 */
static b3level_t *readLevel(btstring_t *data)
{
	uint8_t		*buf;
	int8_t		i, j;
	b3level_t	*r;

	buf = data->buf;
	r = (b3level_t *)xzalloc(sizeof(b3level_t));

	for (i = 0; i < 16; i++) {
		r->name[i] = *buf++ & 0x7f;
		if (r->name[i] == 0x7f)
			r->name[i] = '\0';
	}

	r->levFlags = *buf++;
	r->levelNum = *buf++;

	for (i = 0; i < 8; i++)
		r->dunLevel[i] = *buf++;

	r->exitSqN = *buf++;
	r->exitSqE = *buf++;
	r->exitLoc = *buf++;
	r->monIndex = *buf++;
	r->width = *buf++;
	r->height = *buf++;
	r->deltaSqE = *buf++;
	r->deltaSqN = *buf++;
	r->dataOffset = str_read16le(buf);
	buf += sizeof(uint16_t);

	r->rowOffset = (uint16_t *) xzalloc(r->height * sizeof(uint16_t));
	r->squares = (b3dunSq_t **) xzalloc(r->height * sizeof(b3dunSq_t *));
	for (i = 0; i < r->height; i++) {
		r->rowOffset[i] = str_read16le(buf);
		buf += sizeof(uint16_t);
		r->squares[i] = (b3dunSq_t *) xzalloc(r->width * sizeof(b3dunSq_t));
	}

	for (i = (r->height - 1); i >= 0; i--) {
		for (j = 0; j < r->width; j++) {
			r->squares[i][j].walls = str_read16le(buf);
			buf += sizeof(uint16_t);
			r->squares[i][j].flags = str_read16le(buf);
			buf += sizeof(uint16_t);
			r->squares[i][j].field_4 = *buf++;
		}
	}
	
	r->dataCount = *buf++;
	r->dataList = (b3data_t *) xmalloc(r->dataCount * sizeof(b3data_t));
	for (i = 0; i < r->dataCount; i++) {
		r->dataList[i].sqN = *buf++;
		r->dataList[i].sqE = *buf++;
		r->dataList[i].offset = str_read16le(buf);
		buf += sizeof(uint16_t);
	}

	r->dataBaseOffset = buf - data->buf;
	r->codeBuf = bts_strncpy(buf, data->size - r->dataBaseOffset);

	return r;
}



/*
 * readData()
 */
static btstring_t *readData(uint8_t levNumber)
{
	FILE		*fp;
	btstring_t	*data;
	btstring_t	*rval;

	if (levNumber & HIFLAG)
		fp = xfopen(mkBardThreePath("MAPS.HI"), "rb");
	else
		fp = xfopen(mkBardThreePath("MAPS.LO"), "rb");

	fp_moveToIndex16(fp, levNumber & ~HIFLAG, 0);
	data	= bts_new(19712);

	xfread(data->buf, 1, data->size, fp);
	fclose(fp);

	rval	= d3comp(data->buf, data->size);
	bts_free(data);


	return rval;
}

/********************************/
/*				*/
/* Public Interface		*/
/*				*/
/********************************/

/*
 * readMap()
 */
b3level_t *readMap(uint8_t levelNumber)
{
	btstring_t	*data;
	b3level_t	*level;

	data	= readData(levelNumber);
	level	= readLevel(data);
	bts_free(data);

	return level;
}

/*
 * freeLevel()
 */
void freeLevel(b3level_t *l)
{
	uint32_t	i;

	if (l == NULL)
		return;

	for (i = 0; i < l->height; i++)
		free(l->squares[i]);

	free(l->squares);
	free(l->rowOffset);
	bts_free(l->codeBuf);
	free(l);
}

