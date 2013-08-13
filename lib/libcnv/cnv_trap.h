#ifndef _CNV_TRAP_H
#define _CNV_TRAP_H

#include <cnvlib.h>

/********************************/
/*				*/
/* trap_t structure		*/
/*				*/
/********************************/

typedef struct {
	btstring_t *name;
	btstring_t *effectString;
	uint8_t ndice;
	uint8_t dieval;
	uint8_t spAttack;
	uint8_t sv_lo;
	uint8_t sv_hi;
	uint8_t partyFlag;
	uint8_t sv_chance;
	uint8_t sv_half;
} trap_t;

/********************************/
/*				*/
/* Public interface		*/ 
/*				*/
/********************************/

cnvList_t	*trapList_new(void);
json_t		*trapList_toJson(cnvList_t *tl);

trap_t		*trap_new(void);

#endif
