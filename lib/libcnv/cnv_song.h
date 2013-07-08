#ifndef _LIBCNV_CNV_SONG_H
#define _LIBCNV_CNV_SONG_H

#include <cnv_effect.h>
#include <cnv_function.h>

/********************************/
/*				*/
/* btSong_t structure		*/
/*				*/
/********************************/

typedef struct {
	btstring_t	*name;
	btFunction_t	*activate;
	btFunction_t	*deactivate;
	btFunction_t	*combatFunction;
	cnvList_t	*combatData;
	cnvList_t	*nonCombatData;
} btSong_t;

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

cnvList_t	*songList_new(void);
void		songList_toJson(cnvList_t *sl, btstring_t *fname);

btSong_t	*btSong_new(void);

#endif
