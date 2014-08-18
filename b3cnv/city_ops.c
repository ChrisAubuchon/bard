#include <b3lib.h>
#include <dun.h>
#include <cnv_dun.h>
#include <cnv_city.h>

#include <gl_list.h>
#include <gl_linkedhash_list.h>

#define DEBUG
#include <debug.h>


/****************************************/
/*					*/
/* Global variables			*/
/*					*/
/****************************************/

/****************************************/
/*					*/
/* Local function prototypes		*/
/*					*/
/****************************************/

static btstring_t	*readData(uint8_t);
static b3city_t		*__readCity(btstring_t *);

/****************************************/
/*					*/
/* Local functions			*/
/*					*/
/****************************************/

/*
 * readData()
 */
static btstring_t *readData(uint8_t levNumber)
{
	FILE		*fp;
	btstring_t	*data;
	btstring_t	*rval;
	uint8_t		cityNumber;

	cityNumber	= cityList[levNumber].levels[0];

	if (cityNumber & HIFLAG)
		fp = xfopen(mkBardThreePath("MAPS.HI"), "rb");
	else
		fp = xfopen(mkBardThreePath("MAPS.LO"), "rb");

	fp_moveToIndex16(fp, cityNumber & ~HIFLAG, 0);
	data	= bts_new(19712);

	xfread(data->buf, 1, data->size, fp);
	fclose(fp);

	rval	= d3comp(data->buf, data->size);
	bts_free(data);


	return rval;
}

/*
 * readCity()
 */
static b3city_t *__readCity(btstring_t *data)
{
	uint32_t	i, j;
	uint8_t		*buf;
	b3city_t	*rval;

	buf = data->buf;
	rval = xzalloc(sizeof(b3city_t));

	for (i = 0; i < 16; i++) {
		rval->name[i] = *buf++ & 0x7f;
		if (rval->name[i] == 0x7f)
			rval->name[i] = '\0';
	}

	rval->width		= *buf++;
	rval->height		= *buf++;
	rval->wrapFlag		= *buf++;
	rval->monsterIndex	= *buf++;
	rval->levFlags		= *buf++;
	rval->dataOffset	= str_read16le(buf);
	buf			+= sizeof(uint16_t);

	rval->rowOffset		= (uint16_t *)xzalloc(
					rval->height * sizeof(uint16_t)
				);
	rval->squares		= (uint8_t **)xzalloc(
					rval->height * sizeof(uint8_t *)
				);

	for (i = 0; i < rval->height; i++) {
		rval->rowOffset[i]	= str_read16le(buf);
		buf			+= sizeof(uint16_t);
		rval->squares[i]	= (uint8_t *)xzalloc(
						rval->width * sizeof(uint8_t)
					);
	}

	for (i = 0; i < rval->height; i++) {
		for (j = 0; j < rval->width; j++) {
			rval->squares[i][j] = data->buf[rval->rowOffset[i] + j];
			buf++;
		}
	}

	rval->dataCount = *buf++;
	rval->dataList = (b3data_t *)xzalloc(rval->dataCount * sizeof(b3data_t));
	for (i = 0; i < rval->dataCount; i++) {
		rval->dataList[i].sqN = *buf++;
		rval->dataList[i].sqE = *buf++;
		rval->dataList[i].offset = str_read16le(buf);
		buf += sizeof(uint16_t);
	}

	rval->dataBaseOffset = buf - data->buf;
	rval->codeBuf = bts_strncpy(buf, data->size - rval->dataBaseOffset);

	return rval;
}

/********************************/
/*				*/
/* Public Interface		*/
/*				*/
/********************************/

b3city_t *readCity(uint8_t cityNumber)
{
	btstring_t	*data;
	b3city_t	*city;

	data	= readData(cityNumber);
	city	= __readCity(data);

	bts_free(data);

	return city;
}

/*
 * freeCity()
 */
void freeCity(b3city_t *c)
{
	uint32_t	i;

	for (i = 0; i < c->height; i++)
		free(c->squares[i]);

	free(c->squares);
	free(c->rowOffset);
	bts_free(c->codeBuf);
	free(c);
}

