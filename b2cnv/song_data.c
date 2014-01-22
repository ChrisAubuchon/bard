#include <b2lib.h>

uint8_t *song_combatStrings[] = {
	"local missile_damage_bonus_penalty = true",
	"currentBattle.song:addBattleBonus(\"antiMagic\", 2, true)", 
	"party.acBonus = party.acBonus + inAction.inData.data",
	"currentBattle.song:addBattleBonus(\"toHitBonus\",\
				inAction.inData.data,\
				true);\
	 currentBattle.song:addBattleBonus(\"damageBonus\", \
				inAction.inData.data,\
				true);",
	"currentBattle.song:addBattleBonus(\"hpRegen\", inAction.inData.data, \
				true)",
	"currentBattle.song:addBattleBonus(\"extraAttacks\", 1, false)",
	""
};

uint8_t *song_activateStrings[] = {
	"",
	"party.battle.antiMagic = party.battle.antiMagic + 3",
	"party.song.acBonus = party.song.acBonus + inAction.inData.data; party:display()",
	"party:meleeMarch()",
	"party.song.trapProtect = true", 
	"party.song.spptRegen = true",
	"spells.passive.light(inAction); party.song.lightSong = true"
};

uint8_t *song_deactivateStrings[] = {
	"",
	"party.battle.antiMagic = 0",
	"party.song.acBonus = 0; party:display()",
	"",
	"party.song.trapProtect = false",
	"party.song.spptRegen = false",
	"party.light:deactivate(); party.song.lightSong = false"
};

uint8_t *songName[] = {
	"Archer's Tune",
	"Spellsong",
	"Sanctuary Score",
	"Melee March",
	"Zanduvar Carack",
	"Rhyme of Duotime",
	"Watchwood Melody"
};

uint8_t songBonusList[] = {
	1, 1, 2, 2,
	2, 3, 3, 4
};

uint8_t meleeMarchBonusList[] = {
	1, 1, 2, 4,
	6, 8, 10, 12
};
