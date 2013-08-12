#include <b1lib.h>
#include <dun.h>

b1levData_t b1dungeons[] = {
  { "cellars", 4, { 0, 1, 2, 3, -1 }},
  { "catacombs", 3, {4, 5, 6 , -1, -1}},
  { "castle", 3, {7, 8, 9, -1, -1 }},
  { "kylr", 1, {10, -1, -1, -1, -1 }},
  { "mangar", 5, {11, 12, 13, 14, 15 }}
};

uint8_t b1dun_randomMask[] = {
	0, 15, 15, 31, 31,
	31, 31, 31, 31
};

uint8_t b1dun_randomOffset[] = {
	0, 0, 8, 8, 16,
	32, 48, 64, 81
};

uint8_t b1dun_poisonDamage[] = {
	0, 1,2,4,6,
	8,10,16,20
};

uint8_t *b1dun_specialCode[] = {
	"--hitTrap",
	"--doNothing",
	"--doNothing",
	"--doNothing",
	"--doNothing",
	"--doNothing",
	"currentLevel:longStairs()",
	"currentLevel:spiderGod()",
	"currentLevel:lightBeam()",
	"currentLevel:tarjanMouth()",
	"currentLevel:basharKavilor()",
	"currentLevel:sphynx()",
	"currentLevel:witchKing()",
	"--doNothing",
	"currentLevel:baronsThrone()",
	"currentLevel:guardCaptain()",
	"currentLevel:robedMen()",
	"currentLevel:crystalSword()",
	"currentLevel:vampireOldMan()",
	"currentLevel:silverSquare()",
	"currentLevel:shieldsMouth()",
	"currentLevel:partyHasRobes()",
	"currentLevel:eyeStatue()",
	"currentLevel:skullTavernQuestion()",
	"--doNothing",
	"--doNothing",
	"--doNothing",
	"currentLevel:stoneGolemMouth()",
	"currentLevel:sinisterMouth()",
	"currentLevel:silverTriangle()",
	"currentLevel:crystalStatue()",
	"currentLevel:onyxKey()",
	"--doNothing",
	"currentLevel:mouthFour()",
	"currentLevel:silverCircle()",
	"currentLevel:masterKey()",
	"currentLevel:sevenAnswers()",
	"currentLevel:largeCoffin()",
	"currentLevel:sleepingDragons()",
	"currentLevel:thorFigurine()",
	"currentLevel:swapDoors()",
	"currentLevel:silverDest()",
	"currentLevel:spectreSnare()",
	NULL,
	"currentLevel:boilingLiquid()",
	"currentLevel:mangarTreasure()",
	"currentLevel:mouthOne()",
	"currentLevel:possessCharacter()",
	"currentLevel:mangar()"
};
