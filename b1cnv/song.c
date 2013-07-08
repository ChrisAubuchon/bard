#include <b1lib.h>
#include <song.h>
#include <spell.h>
#include <cnv_song.h>

/********************************/
/*				*/
/* Internal prototypes		*/
/*				*/
/********************************/

static void	song_lightSpell(btSong_t *bs);
static void	song_generic(cnvList_t *list);

/********************************/
/*				*/
/* Internal functions		*/
/*				*/
/********************************/

static void song_lightSpell(btSong_t *bs)
{
	btEffect_t	*effect;
	btePassive_t	*bp;
	uint32_t	i;
	uint32_t	offset;

	bs->activate = btFunction_new(FUNC_STRING,
				bts_strcpy(song_activateStrings[1]));
	bs->deactivate = btFunction_new(FUNC_STRING,
				bts_strcpy(song_deactivateStrings[1]));

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

static void song_generic(cnvList_t *list)
{
	btEffect_t	*effect;
	bteGeneric_t	*bg;
	uint32_t	i;

	for (i = 0; i < 8; i++) {
		effect = btEffect_new(EFFECT_GENERIC);
		bg = btEffect_generic(effect);

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

	for (i = 0; i < 6; i++) {
		b = btSong_new();
		b->name = bts_strcpy(songName[i]);

		switch (i) {
		case 1:
			song_lightSpell(b);
			/* Fall through */
		case 0:
		case 2:
		case 3:
		case 5:
			b->combatFunction = btFunction_new(FUNC_STRING,
					bts_strcpy(song_combatStrings[i]));
			song_generic(b->combatData);
			break;
		case 4:
			b->activate = btFunction_new(FUNC_STRING,
					bts_strcpy(song_activateStrings[i]));
			b->deactivate = btFunction_new(FUNC_STRING,
					bts_strcpy(song_deactivateStrings[i]));
			song_generic(b->nonCombatData);
			b->combatFunction = btFunction_new(FUNC_STRING,
					bts_strcpy(song_combatStrings[i]));
			song_generic(b->combatData);
			break;
		}

		cnvList_add(songs, b);
	}

	songList_toJson(songs, mkJsonPath("songs.json"));
	cnvList_free(songs);
}






















