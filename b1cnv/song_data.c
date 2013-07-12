#include <b1lib.h>

uint8_t *song_combatStrings[] = {
	"local inBattle = inAction.inBattle; inBattle.songDamageBonus = inBattle.songDamageBonus + inAction.inData.data",
	"local inBattle = inAction.inBattle; inBattle.songToHitBonus = inBattle.songToHitBonus + inAction.inData.data",
	"local inBattle = inAction.inBattle; inBattle.songToHitPenalty = inBattle.songToHitPenalty + inAction.inData.data",
	"inAction.inBattle.songHpRegen = true",
	"party.battle.acBonus = party.battle.acBonus + inAction.inData.data",
	"party.battle.antiMagic = party.battle.antiMagic + 2"
};

uint8_t *song_activateStrings[] = {
	"",
	"spells.passive.light(inAction); party.song.lightSong = true",
	"",
	"", 
	"party.song.acBonus = party.song.acBonus + inAction.inData.data; party:display()",
	""
};

uint8_t *song_deactivateStrings[] = {
	"",
	"party.light:deactivate(); party.song.lightSong = false",
	"",
	"",
	"party.song.acBonus = 0; party:display()",
	"",
};
