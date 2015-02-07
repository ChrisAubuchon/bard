#ifndef _B3_DUN_H
#define _B3_DUN_H

#include <bt_string.h>
#include <dehuf.h>

#define SQ_STAIRS               0x0001
#define SQ_SPECIAL              0x0004
#define SQ_DARKNESS             0x0008
#define SQ_TRAP                 0x0010
#define SQ_PORTBELOW            0x0020
#define SQ_PORTABOVE            0x0040
#define SQ_HIGHBATTLE           0x0080
#define SQ_SPINNER              0x0100
#define SQ_ANTIMAGIC            0x0200
#define SQ_DRAINHP              0x0400
#define SQ_ODD                  0x0800
#define SQ_SILENCE              0x1000
#define SQ_REGENSPPT            0x2000
#define SQ_DRAINSPPT            0x4000
#define SQ_MONHOSTILE           0x8000

#define SQ_EXPLOSION            0x10
#define SQ_REGENHP             0x40    /* Party heal square. Doesn't seem to have any effect. */
#define SQ_STUCK                0x80

#define setFlag(xLval, xLevel, xY, xX, xFlag) \
	do {						\
		xLval = (xLevel->squares[(xY)][(xX)].flags & xFlag) ? 1 : 0; \
	} while (0)
#define setXFlag(xLval, xLevel, xY, xX, xFlag) \
	do {						\
		xLval = (xLevel->squares[(xY)][(xX)].field_4 & xFlag) ? 1 : 0; \
	} while (0)

#define WILD_CAMP       0x10
#define WILD_TAVERN     0x20
#define WILD_TEMPLE     0x30
#define WILD_BUILDING   0x40
#define WILD_STORAGE    0x50
#define WILD_REVIEW     0x60
#define WILD_WIZARDHALL 0x70
#define WILD_BARDSHALL  0x80

#define HIFLAG		0x80

typedef struct {
  uint8_t dun;
  uint8_t lev;
  uint8_t ncode;
  uint16_t code_offset;
} b3dunFile_t;

typedef struct {
  uint16_t mon_lo;
  uint16_t mon_hi;
} b3monIndex_t;

#if 0
typedef struct {
  char *name;
  uint8_t wflag;
  uint8_t nlevs;
  b3dunFile_t files[5];
} b3dunData_t;
#endif

typedef struct {
	uint8_t		*name;
	uint8_t		*levels;
} b3dunData_t;

typedef struct {
  uint8_t sqN;
  uint8_t sqE;
  uint16_t offset;
} b3data_t;

typedef struct {
  uint16_t flags;
  uint16_t walls;
  uint8_t field_4;
} b3dunSq_t;

typedef struct {
  uint8_t name[16];
  uint8_t levFlags;
  uint8_t levelNum;
  uint8_t dunLevel[8];
  uint8_t exitSqN;		/* Replaced by a different stairs() function */
  uint8_t exitSqE;;		/* Replaced by a different stairs() function */
  uint8_t exitLoc;;		/* Replaced by a different stairs() function */
  uint8_t monIndex;
  uint8_t width;
  uint8_t height;
  int8_t deltaSqE;
  int8_t deltaSqN;
  uint16_t dataOffset;
  uint16_t *rowOffset;
  b3dunSq_t **squares;
  uint8_t dataCount;
	uint16_t	dataBaseOffset;
  b3data_t *dataList;
  btstring_t *codeBuf;

	uint16_t	codeStartOffset;
} b3level_t;

typedef struct {
  uint8_t name[16];
  uint8_t width;
  uint8_t height;
  uint8_t wrapFlag;
  uint8_t monsterIndex;
  uint8_t levFlags;
  uint16_t dataOffset;
  uint16_t *rowOffset;
  uint8_t **squares;
  uint8_t dataCount;
	uint16_t	dataBaseOffset;
  b3data_t *dataList;
  btstring_t *codeBuf;

	/* Helper data	*/
	uint8_t		isCity;
} b3city_t;


extern b3dunData_t	cityList[];
extern	b3dunData_t duns[];
extern	b3monIndex_t dun_monIndexList[];
extern	uint8_t levno2dun[];
extern	uint8_t levno2lev[];
extern	uint8_t dun_entryTypeList[];
extern	uint8_t dun_entrySqN[];
extern	uint8_t dun_entrySqE[];
extern	uint8_t dun_trapSave[];
extern	uint8_t dun_poisonDmg[];
extern	uint8_t dun_items[];
extern	uint8_t dun_styleMap[];
extern	uint8_t dun_wallFlag[];

extern uint8_t	city_referenceIndex[];
extern uint8_t	*city_referenceLocation[];

void		addItems(cnvList_t *, uint8_t);
void		addMonsters(cnvList_t *, uint8_t);
b3level_t	*readMap(uint8_t);
void		freeLevel(b3level_t *);

b3city_t	*readCity(uint8_t);
void		freeCity(b3city_t *);

extern uint8_t		*tileSetList[];

#endif
