#ifndef __CNV_PARAM_H
#define __CNV_PARAM_H


#include <cnv_list.h>
#include <stdarg.h>

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define PARAM_NONE		0
#define PARAM_STRING		1
#define PARAM_BTSTRING		2
#define PARAM_NUMBER		3
#define PARAM_BOOL		4
#define PARAM_LIST		5

#define paramList_t		cnvList_t


/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

paramList_t		*paramList_new(void);
void			paramList_free(const void *);
json_t			*paramList_toJson(paramList_t *);
void			param_add(paramList_t *, uint32_t, uint8_t *, ...);



#endif
