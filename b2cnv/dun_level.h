#ifndef _DUN_LEVEL_H
#define _DUN_LEVEL_H

#define LEVEL_X		0x16
#define LEVEL_Y		0x16
#define LEVEL_SIZE (LEVEL_X*LEVEL_Y)

#define WALL_NORTH	0x01
#define WALL_SOUTH	0x04
#define WALL_EAST	0x10
#define WALL_WEST	0x40
#define DOOR_NORTH	0x02
#define DOOR_SOUTH	0x08
#define DOOR_EAST	0x20
#define DOOR_WEST	0x80

#define NORTH_MASK	0x03
#define SOUTH_MASK	0x0c
#define EAST_MASK	0x30
#define WEST_MASK	0xc0

#define DUN_UPSTAIR	0x01
#define DUN_DOWNSTAIR	0x02
#define DUN_SPECIAL	0x04
#define DUN_DARKNESS	0x08
#define DUN_TRAP	0x10
#define DUN_FLOORPORT	0x20
#define DUN_CEILPORT	0x40
#define DUN_BATTLE	0x80

#define STAIR_MASK	(DUN_UPSTAIR | DUN_DOWNSTAIR)

#define HI_SPINNER	0x01
#define HI_ANTIMAGIC	0x02
#define HI_DAMAGE	0x04	
#define HI_SOMETHINGODD	0x08
#define HI_ENDTUNE	0x10
#define HI_SPREGEN	0x20
#define HI_DRAINSPPT	0x40
#define HI_HOSTILEMON	0x80

/*
 * Dungeon level structure
 * Bytes:
 * 0 - 0x1e3		A 22x22 layout of the dungeon
 *			Each byte is an OR'd value describing the walls and
 *			doors around the square.
 *			0x01 - North wall
 *			0x02 - North door
 *			0x04 - South wall
 *			0x08 - South door
 *			0x10 - East wall
 *			0x20 - East door
 *			0x40 - West wall
 *			0x80 - West door
 * 0x1e4 - 0x1ff	Unused
 * 0x200 - 0x3e3	22x22 Lo byte for square flags
 *			0x01 -
 *			0x02 -
 *			0x04 -
 *			0x08 - Darkness!
 *			0x10 -
 *			0x20 - Portal on floor
 *			0x40 - Portal on ceiling
 *			0x80 -
 * 0x3e4 - 0x3ff	Unused
 * 0x400 - 0x4e3	22x22 Hi byte for square flags
 * 0x4e4 - 0x5ff	Unused
 * 0x600 - 0x60d	Level Name AND'd with 0x80
 * 0x60e		curLevelForSong value
 * 0x60f		
 * 0x610 		Dungeon direction. 
 * 0x611
 * 0x612
 * 0x613		Squares north upon exiting
 * 0x614		Squares east upon exiting
 * 0x615		Location number upon exiting
 * 0x616		Unknown
 * 0x617		Current level canTeleportTo flag
 * 0x618 - 0x61f	Array with canTeleportTo flags
 * 0x620 - 0x62f	Coordinates for magic mouths (sqNorth, sqEast) pairs
 * 0x630 - 0x63f	Special square index number (index, unknown) pairs
 * 0x640 - 0x64f	Coordinates for teleporters (sqNorth, sqEast) pairs
 * 0x650 - 0x65f	Destination coordinates for teleporters (sqNorth, sqEast) pairs
 * 0x660 - 0x66f	Coordinates for required battles (sqNorth, sqEast) pairs
 * 0x670 - 0x67f	Monster group/size pairs (name, size) pairs
 * 0x680 - 0x68f	Coordinates for informational messages (sqNorth, sqEast) pairs
 * 0x690 - 0x69f	Little Endian offset into the file for the text message
 * 0x610 - 0x69f	Unknown level data
 * 0x6a0 - 0x900	Text for level events AND'd with 0x80
 */

typedef struct {
  uint8_t sqNorth;
  uint8_t sqEast;
} _dunCoord_t;

typedef struct {
  uint8_t sqNumber;
  uint8_t unknown;
} _magicMouth_t;

typedef struct {
	uint8_t		monName;
	uint8_t		monSize;
} _batGroup_t;

typedef struct {
	uint8_t		wallData[0x200];
	uint8_t		loFlags[0x200];
	uint8_t		hiFlags[0x200];
	uint8_t		title[14];
	uint8_t		songLevel;
	uint8_t		unknown;
	uint8_t		direction;
	uint8_t		unknown5[2];
	uint8_t		exitSQN;
	uint8_t		exitSQE;
	uint8_t		exitLocation;
	uint8_t		unknown2;
	uint8_t		phaseFlag;
	uint8_t		canTeleportToFlag[8];
	_dunCoord_t	magicMouth[8];
	_magicMouth_t	mouthNum[8];
	_dunCoord_t	teleporter[8];
	_dunCoord_t	teleDest[8];
	_dunCoord_t	reqBattle[8];
	_batGroup_t	reqBatMon[8];
	_dunCoord_t	infoMess[8];
	uint16_t	infoOffset[8];
	uint8_t		text[0x260];
} dungeonLevel_t;

/*
 * Dungeon monster structure
 * Length: 0x20
 * 0x00 - 0x0f		Name AND'd with 0x80
 * 0x10 - 0x1f
 */
typedef struct {
  uint8_t name[16];
  uint16_t hpRandom;
  uint16_t hpBase;
  uint8_t maxGrpSize;
  uint8_t advanceSpeed;
  uint8_t baseToHit;
  uint8_t numAttacks;
  uint8_t attackTypes[4];
  uint8_t meleeAttType;
  uint8_t baseMeleeDmg;
  uint8_t picIndex;
  uint8_t flags;
} _dungeonMonst_t;

/*
 * Dungeon Level Monster file structure
 * 0x300 bytes in length
 * An array of 0x20 byte long monster structures
 */
typedef struct {
  _dungeonMonst_t monster[24];
} dungeonLevelMonst_t;

#define setSquareMember(inDl, inX, inY, flag, value)		\
	do {							\
		dunVertex_t *v;					\
		v = dunVertex_get(inDl, getVertex(inX, inY));	\
		v->flag = value;				\
	} while(0)
#define setSquareMemberIfSpecial(inDl, inX, inY, flag, value)	\
	do {							\
		dunVertex_t *v;					\
		v = dunVertex_get(inDl, getVertex(inX, inY));	\
		if (v->isSpecial)				\
			v->flag = value;			\
	} while(0)
#define setSquareFlag(inDl, inX, inY, flag)			\
	setSquareMember(inDl, inX, inY, flag, 1)

#endif

