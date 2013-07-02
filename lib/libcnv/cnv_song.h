#ifndef _LIBCNV_CNV_SONG_H
#define _LIBCNV_CNV_SONG_H

#include <cnv_effect.h>

typedef struct {
	btFunction_t	*activate;
	btFunction_t	*deactivate;
	btFunction_t	*combatFunction;
	cnvList_t	*combatData;
	cnvList_t	*nonCombatData;
} btSong_t;

#endif
