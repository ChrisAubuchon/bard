#include <b1lib.h>

uint8_t *song_combatStrings[] = {
	"local inBattle = inAction.inBattle; inBattle.songDamageBonus = inBattle.songDamageBonus + inAction.inData.data",
	"local inBattle = inAction.inBattle; inBattle.songToHitBonus = inBattle.songToHitBonus + inAction.inData.data",
	"local inBattle = inAction.inBattle; inBattle.songToHitPenalty = inBattle.songToHitPenalty + inAction.inData.data",
	"inAction.inBattle.songHpRegen = true",
	"party.spellAcBonus = party.spellAcBonus + inAction.inData.data",
	"party.antiMagicBonus = party.antiMagicBonus + 2"
};

uint8_t *song_activateStrings[] = {
	"",
	"spells.passive.light(inAction); party.lightSongPlaying = true",
	"",
	"", 
	"party.songAcBonus = party.songAcBonus + inAction.inData.data; party:display()",
	""
};

uint8_t *song_deactivateStrings[] = {
	"",
	"party.light.deactivate(); party.lightSongPlaying = false",
	"",
	"",
	"party.songAcBonus = 0; party:display()",
	"",
};
