#include <b2lib.h>
#include <dun.h>

b2levData_t b2dungeons[] = {
	{ "domain",	4, { 0,  1,  2,  3, -1 }},
	{ "tombs",	3, { 4,  5,  6, -1, -1 }},
	{ "castle",	1, { 7, -1, -1, -1, -1 }},
	{ "tower", 	5, { 8,  9, 10, 11, 12 }},
	{ "maze",	3, {13, 14, 15, -1, -1 }},
	{ "fort",	4, {16, 17, 18, 19, -1 }},
	{ "crypt",	2, {20, 21, -1, -1, -1 }},
	{ "stone",	3, {22, 23, 24, -1, -1 }}
};

uint8_t *b2dun_specialCode[] = {
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
