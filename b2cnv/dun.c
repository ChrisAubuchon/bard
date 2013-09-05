#include <b2lib.h>
#include <dun_level.h>
#include <dehuf.h>

#define DEBUG
#include <debug.h>

/********************************/
/*				*/
/* Internal function prototypes	*/
/*				*/
/********************************/

static FILE	*openLevelFile(void);

/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/

static FILE *openLevelFile(void)
{
	return xfopen(mkBardTwoPath("LEVS"), "rb");
}

/********************************/
/*				*/
/* Public functions		*/ 
/*				*/
/********************************/

uint32_t getMaxLevels(void)
{
	FILE		*fp;
	uint32_t	rval;

	fp = openLevelFile();

	rval = fp_read32le(fp) / 4;

	fclose(fp);

	return rval;
}

btstring_t *getMonsters(uint32_t level)
{
	FILE		*fp;
	huffile_t	*huf;
	btstring_t	*data;

	fp = openLevelFile();
	fp_moveToIndex32(fp, level, 0);

	huf = dehuf_init(fp);
	data = dehuf(huf, 0x900);
	bts_free(data);
	data = dehuf(huf, 0x300);

	fclose(fp);

	dehuf_free(huf);

	return data;
	
}
