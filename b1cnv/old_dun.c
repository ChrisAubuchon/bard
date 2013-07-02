#include <stdio.h>
#include <stdint.h>
#include <fileio.h>
#include <dun_level.h>

static dungeonLevel_t level;
static dungeonLevelMonst_t monst;

static void printSpecialSquare(int index);
static void printSpecialSquare(int index)
{
  uint16_t sqN, sqE;
  int i;
  int offset;

  sqN = index / LEVEL_Y;
  sqE = index % LEVEL_X;

  for (i = 0; i < 8; i++) {
    if ((sqN == level.dun_spSquare[i].sqNorth) && 
	(sqE == level.dun_spSquare[i].sqEast)) {
      printf("  Magic Mouth index %d\n", level.dun_spSqOff[i].sqNumber - 0xa);
      break;
    }

    if ((sqN == level.dun_teleporter[i].sqNorth) &&
	(sqE == level.dun_teleporter[i].sqEast)) {
      printf("  Teleporter destination (%d, %d)\n", \
		level.dun_teleDest[i].sqNorth, \
		level.dun_teleDest[i].sqEast);
      break;
    }

    if ((sqN == level.dun_infoMess[i].sqNorth) &&
	(sqE == level.dun_infoMess[i].sqEast)) {
      printf("  Informational message '");
      offset = level.dun_infoOffset[i] - 0xf800 - 0x520;
      while ((level.dun_text[offset] & 0x7f) != 0x7f) {
	if ((level.dun_text[offset] & 0x7f) == '\\')
	  break;
	if (level.dun_text[offset] & 0x7f) 
	  printf("%c", level.dun_text[offset] & 0x7f);
	else
	  printf("\n");
	offset++;
      }
      printf("'\n");
      break;
    }

    if ((sqN == level.dun_spinner[i].sqNorth) &&
	(sqE == level.dun_spinner[i].sqEast)) {
      printf("  Spinner square\n");
    }

    if ((sqN == level.dun_smoke[i].sqNorth) &&
	(sqE == level.dun_smoke[i].sqEast)) {
      printf("  Smoke screen square\n");
    }

    if ((sqN == level.dun_noSpptRegen[i].sqNorth) &&
	(sqE == level.dun_noSpptRegen[i].sqEast)) {
      printf("  No sppt regen square\n");
    }

    if ((sqN == level.dun_reqBattle[i].sqNorth) &&
	(sqE == level.dun_reqBattle[i].sqEast)) {
      printf("  Required battle square\n");
      printf("    Monster type: %d\n", level.dun_reqBatMon[i].monName);
      printf("    Monster size: %d\n", level.dun_reqBatMon[i].monSize);
    }

    if ((sqN == level.dun_antiMagic[i].sqNorth) &&
	(sqE == level.dun_antiMagic[i].sqEast)) {
      printf("  Anti-magic square\n");
    }

    if ((sqN == level.dun_antiMagic[i<<1].sqNorth) &&
	(sqE == level.dun_antiMagic[i<<1].sqEast)) {
      printf("  Anti-magic square\n");
    }

    if ((sqN == level.dun_lifeDrain[i].sqNorth) &&
	(sqE == level.dun_lifeDrain[i].sqEast)) {
      printf("  Life drain square\n");
    }

    if ((sqN == level.dun_lifeDrain[i<<1].sqNorth) &&
	(sqE == level.dun_lifeDrain[i<<1].sqEast)) {
      printf("  Anti-magic square\n");
    }

    if ((sqN == level.dun_teleonly[i].sqNorth) &&
	(sqE == level.dun_teleonly[i].sqEast)) {
      printf("  Teleport only square\n");
      break;
    }
  }
}

int main(int argc, char *argv[])
{
  int fd;
  int i, j;
  char buf[512];

  if (argc == 2) {
    sprintf(buf, "lev%s.decomp", argv[1]);
  } else {
    sprintf(buf, "lev0.decomp");
  }

  fd = xopen(buf, O_RDONLY);
  xread(fd, &level, sizeof(dungeonLevel_t));
  close(fd);

  for (i = 0; i < 10; i++) {
    level.dun_title[i] ^= 0x80;
    if (level.dun_title[i] == '\\')
      level.dun_title[i] = '\0';
  }
  level.dun_title[9] = '\0';

  printf("Level name: '%s'\n", level.dun_title);
  printf("Monster Level: %d\n", level.dun_monstLevel);
  printf("Direction: %d\n", level.dun_direction);
  printf("Squares North upon exit: %d\n", level.dun_exitSqN);
  printf("Squares East upon exit: %d\n", level.dun_exitSqE);
  printf("Exit Direction: %d\n", level.dun_exitDir);
  printf("Phase door flag: %d\n", level.dun_phaseFlag);

  for (i = 0; i < LEVEL_SIZE; i++) {
    printf("SqNorth = %d, sqEast = %d:\n", (i / LEVEL_Y), (i % LEVEL_X));

    /* Print wall & door information */
    if (level.wallData[i] & NORTH_MASK) {
      printf("  North: %s\n", (level.wallData[i] & WALL_NORTH) ? ((level.wallData[i] & DOOR_NORTH) ? "secret-door" : "wall") : "door");
    } else{
      printf("  North: Open\n");
    }
    if (level.wallData[i] & SOUTH_MASK) {
      printf("  South: %s\n", (level.wallData[i] & WALL_SOUTH) ? ((level.wallData[i] & DOOR_SOUTH) ? "secret-door" : "wall") : "door");
    } else{
      printf("  South: Open\n");
    }
    if (level.wallData[i] & EAST_MASK) {
      printf("  East: %s\n", (level.wallData[i] & WALL_EAST) ? ((level.wallData[i] & DOOR_EAST) ? "secret-door" : "wall") : "door");
    } else{
      printf("  East: Open\n");
    }
    if (level.wallData[i] & WEST_MASK) {
      printf("  West: %s\n", (level.wallData[i] & WALL_WEST) ? ((level.wallData[i] & DOOR_WEST) ? "secret-door" : "wall") : "door");
    } else{
      printf("  West: Open\n");
    }

    printf("  Flags (%x):\n", level.dun_flags[i]);
    /* Print special square flags */
    if (level.dun_flags[i] & DUN_DARKNESS)
      printf("    Darkness\n");
    if (level.dun_flags[i] & DUN_FLOORPORT)
      printf("    Floor portal\n");
    if (level.dun_flags[i] & DUN_CEILPORT)
      printf("    Ceiling portal\n");
    if (level.dun_flags[i] & DUN_BATTLE)
      printf("    Up battle probability\n");
    if (level.dun_flags[i] & DUN_TRAP)
      printf("    Trap square\n");
    if (level.dun_flags[i] & STAIR_MASK) {
      uint8_t ax, cx;

      ax = level.dun_direction & 1;
      cx = (level.dun_flags[i] >> 1) & 1;
      ax ^= cx;

      if (ax) 
        printf("    Down stairs\n");
      else
	printf("    Up stairs\n");
    }

    if (level.dun_flags[i] & DUN_SPECIAL)
      printSpecialSquare(i);
  }
}
