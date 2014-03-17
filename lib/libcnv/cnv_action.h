#ifndef _LIBCNV_CNV_ACTION_H
#define _LIBCNV_CNV_ACTION_H

#include <cnv_effect.h>
#include <cnv_function.h>

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define PARAM_NONE		0
#define	PARAM_STRING		1
#define PARAM_BTSTRING		2
#define PARAM_NUMBER		3
#define PARAM_BOOL		4

/********************************/
/*				*/
/* Structures			*/
/*				*/
/********************************/

typedef struct {
	btFunction_t	*function;
	btEffect_t	*effect;
	cnvList_t	*parameters;
} btAction_t;


/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

btAction_t	*btAction_new(uint8_t ftype, uint8_t etype);
void		btAction_free(const void *vba);
json_t		*btAction_toJson(const void *vba);

void		btAction_addParam(btAction_t *ba, uint32_t type,
					btstring_t *name, ...);

#endif

