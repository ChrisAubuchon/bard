#ifndef _CNV_SPELL_H
#define _CNV_SPELL_H

#include <cnvlib.h>
#include <bt_string.h>
#include <cnv_common.h>
#include <cnv_action.h>

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

/********************************/
/*				*/
/* Structures			*/
/*				*/
/********************************/

typedef struct {
	btstring_t	*full;

	cnvList_t	*nonCombat;
	cnvList_t	*combat;
} bardSong_t;

typedef struct {
	btstring_t 	*abbr;
	btstring_t	*full;
	btstring_t	*class;
	uint8_t		level;

	uint8_t		sppt;

	uint8_t		combat;
	uint8_t		noncombat;
	btTargetting_t	targetting;

	btAction_t	*action;
} mageSpell_t;

/********************************/
/*				*/
/* Public Functions		*/
/*				*/
/********************************/

mageSpell_t	*mageSpell_new(uint8_t type);
bardSong_t	*bardSong_new(void);

cnvList_t	*spellList_new(void);
void		spellList_to_json(cnvList_t *sl, btstring_t *fname);

cnvList_t	*songList_new(void);
void		songList_to_json(cnvList_t *sl, btstring_t *fname);

#endif
