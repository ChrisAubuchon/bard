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
#define HI_SOMETHINGOOD	0x08
#define HI_ENDTUNE	0x10
#define HI_NOSPREGEN	0x20
#define HI_ZEROSPPT	0x40
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
 * 0x400 - 0x407	Unknown
 * 0x408 - 0x40f	Level teleport flag. If 0xff the level cannot be teleported to
 * 0x410		Monster level flag
 * 0x411		Phase door flag. If zero, phase door is allowed.
 * 0x412		Direction upon exit
 * 0x413		Squares north upon exit
 * 0x414		Squares east upon exit
 * 0x416 - 0x41f	Level Name AND'd with 0x80
 * 0x420 - 0x42f	Coordinates for special squares (sqNorth, sqEast) pairs
 * 0x430 - 0x43f	special square index number (index, unknown) pairs
 * 0x440 - 0x45f	Coords for anti-magic squares (sqNorth, sqEast) pairs
 * 0x460 - 0x46f	Coords for teleporters (sqNorth, sqEast) pairs
 * 0x470 - 0x47f	Destination coords for teleporters (sqNorth, sqEast) pairs
 * 0x480 - 0x48f	Coords for spinners (sqNorth, sqEast) pairs
 * 0x490 - 0x49f	Coords for smoke (sqNorth, sqEast) pairs
 * 0x4a0 - 0x4bf	Coords for life drain (sqNorth, sqEast) pairs
 * 0x4c0 - 0x4cf	Coords for no sppt regen (sqNorth, sqEast) pairs
 * 0x4d0 - 0x4df	Coords for 
 * 0x4e0 - 0x4ef	Coords for inform messages (sqNorth, sqEast) pairs
 * 0x4f0 - 0x4ff	Coords for forced battles (sqNorth, sqEast) pairs
 * 0x500 - 0x50f	Monster group/size pairs (name, size) pairs
 * 0x510 - 0x51f	Little endian offset into the file for text messages
 * 0x520 - 0x800	Text for level events AND'd with 0x80
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
  uint8_t monName;
  uint8_t monSize;
} _batGroup_t;

typedef struct {
  uint8_t wallData[0x200];
  uint8_t dun_flags[0x200];
/*
  uint8_t field_400;
  uint8_t field_401;
  uint8_t field_402;
  uint8_t field_403;
  uint8_t field_404;
  uint8_t field_405;
  uint8_t field_406;
  uint8_t field_407;
*/
  uint8_t dun_level[8];
  uint8_t dun_levelTeleFlag[8];
  uint8_t dun_monstLevel;
  uint8_t dun_phaseFlag;
  uint8_t dun_exitDir;
  uint8_t dun_exitSqN;
  uint8_t dun_exitSqE;
  uint8_t dun_direction;
  uint8_t dun_title[10];
  _dunCoord_t dun_spSquare[8];
  _magicMouth_t dun_spSqOff[8];
  _dunCoord_t dun_antiMagic[16];
  _dunCoord_t dun_teleporter[8];
  _dunCoord_t dun_teleDest[8];
  _dunCoord_t dun_spinner[8];
  _dunCoord_t dun_smoke[8];
  _dunCoord_t dun_lifeDrain[16];
  _dunCoord_t dun_spptRegen[8];
  _dunCoord_t dun_teleonly[8];
  _dunCoord_t dun_infoMess[8];
  _dunCoord_t dun_reqBattle[8];
  _batGroup_t dun_reqBatMon[8];
  uint16_t dun_infoOffset[8];
  uint8_t dun_text[0x2e0];
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
  uint8_t field_15;
  uint8_t baseToHit;
  uint8_t field_17;
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
#define setSquareFlag(inDl, inX, inY, flag)			\
	setSquareMember(inDl, inX, inY, flag, 1)

#endif

