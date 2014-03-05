#include <b3lib.h>

static uint8_t *dirStrings[] = {
	"DIR_NORTH", "DIR_EAST", "DIR_SOUTH", "DIR_WEST"
};

static uint8_t *spattackTypes[] = {
  "SPAT_NONE", "SPAT_POISON", "SPAT_LEVDRAIN", "SPAT_INSANITY", "SPAT_OLD",
  "SPAT_POSSESS", "SPAT_STONE", "SPAT_CRITHIT"
};

void printSpecialAttack(uint8_t sp)
{
  printf("%s", spattackTypes[sp]);
}

uint8_t *specialAttack(uint8_t ind)
{
  return spattackTypes[ind];
}

void printElementStru(uint8_t e)
{
  printf("ELEM(%d,%d,%d,%d,%d,%d,%d,%d)", \
	IFBIT(e, 0x80, 1, 0), \
	IFBIT(e, 0x40, 1, 0), \
	IFBIT(e, 0x20, 1, 0), \
	IFBIT(e, 0x10, 1, 0), \
	IFBIT(e, 0x08, 1, 0), \
	IFBIT(e, 0x04, 1, 0), \
	IFBIT(e, 0x02, 1, 0), \
	IFBIT(e, 0x01, 1, 0)  \
	);
}

void printRepelStru(uint8_t r)
{
  printf("REPEL(%d,%d,%d,%d,%d,%d,%d,%d)", \
	IFBIT(r, 0x80, 1, 0), \
	IFBIT(r, 0x40, 1, 0), \
	IFBIT(r, 0x20, 1, 0), \
	IFBIT(r, 0x10, 1, 0), \
	IFBIT(r, 0x08, 1, 0), \
	IFBIT(r, 0x04, 1, 0), \
	IFBIT(r, 0x02, 1, 0), \
	IFBIT(r, 0x01, 1, 0)  \
	);
}


btstring_t *getDirectionMacro(uint8_t dir)
{
	if (dir > 3)
		return NULL;

	return bts_strcpy(dirStrings[dir]);
}
