#include <b2lib.h>
#include <song.h>
#include <spell.h>
#include <cnv_song.h>

#define DEBUG
#include <debug.h>

/********************************/
/*				*/
/* Internal prototypes		*/
/*				*/
/********************************/

static void	song_lightSpell(btSong_t *bs, uint32_t index);
static void	song_generic(cnvList_t *list, uint32_t index);

/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/

static void song_lightSpell(btSong_t *bs, uint32_t index)
{
	btEffect_t	*effect;
	btePassive_t	*bp;
	uint32_t	i;
	uint32_t	offset;

	bs->activate = btFunction_new(FUNC_STRING,
				bts_strcpy(song_activateStrings[index]));
	bs->deactivate = btFunction_new(FUNC_STRING,
				bts_strcpy(song_deactivateStrings[index]));

	for (i = 0; i < 8; i++) {
		offset = songBonusList[i] + 7;

		effect = btEffect_new(EFFECT_PASSIVE);
		bp = btEffect_passive(effect);

		bp->type = PASS_LIGHT;
		bp->duration = lightDur[offset];
		bp->lightDistance = lightDist[offset];
		bp->detectSecret = (lightSDFlag[offset] != 0);

#ifdef DOSBUG
		bp->duration = lightDur[spellDuration[offset]];
		bp->lightDistance = lightDist[spellDuration[offset]];
		bp->detectSecret = (lightSDFlag[spellDuration[offset]] != 0);
#endif

		cnvList_add(bs->nonCombatData, effect);
	}
}

static void song_generic(cnvList_t *list, uint32_t index)
{
	btEffect_t	*effect;
	bteGeneric_t	*bg;
	uint32_t	i;

	for (i = 0; i < 8; i++) {
		effect = btEffect_new(EFFECT_GENERIC);
		bg = btEffect_generic(effect);

		if ((index == 3) || (index == 4)) 
			bg->flags = meleeMarchBonusList[i];
		else
			bg->flags = songBonusList[i];

		cnvList_add(list, effect);
	}
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

void convertSongs(void)
{
	uint32_t	i;
	cnvList_t	*songs;
	btSong_t	*b;

	songs = songList_new();

	for (i = 0; i < 7; i++) {
		b = btSong_new();
		b->name = bts_strcpy(songName[i]);

		switch (i) {
		case 6:
			song_lightSpell(b, i);
			b->combatFunction = btFunction_new(FUNC_STRING,
					bts_strcpy(song_combatStrings[i]));
			break;
		default:
			b->activate = btFunction_new(FUNC_STRING,
					bts_strcpy(song_activateStrings[i]));
			b->deactivate = btFunction_new(FUNC_STRING,
					bts_strcpy(song_deactivateStrings[i]));
			b->combatFunction = btFunction_new(FUNC_STRING,
					bts_strcpy(song_combatStrings[i]));
			song_generic(b->combatData, i);
			song_generic(b->nonCombatData, i);
			break;
		}
		cnvList_add(songs, b);
	}

	songList_toJson(songs, mkJsonPath("songs.json"));
	cnvList_free(songs);
}






















