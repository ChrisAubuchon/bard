#ifndef _CNV_LIST_H
#define _CNV_LIST_H

#include <cnvlib.h>

/********************************/
/*				*/
/* Structures			*/
/*				*/
/********************************/

typedef void		(*cnvList_forEach)(const void *data);

typedef struct cnvList_t cnvList_t;

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

cnvList_t	*cnvList_new(btFunc_free inFree, btFunc_toJson inToJson,
				btFunc_toName inToName);
void		cnvList_free(cnvList_t *cl);
void		cnvList_add(cnvList_t *cl, void *data);
json_t 		*cnvList_toJsonArray(cnvList_t *cl);
json_t 		*cnvList_toJsonObject(cnvList_t *cl);
void		cnvList_runForEach(cnvList_t *cl, cnvList_forEach forEach);
void		cnvList_setToJson(cnvList_t *cl, btFunc_toJson inToJson);
void		cnvList_setToName(cnvList_t *cl, btFunc_toName inToName);
btFunc_toJson	cnvList_getToJson(cnvList_t *cl);
btFunc_toName	cnvList_getToName(cnvList_t *cl);

cnvList_t	*cnvList_btstring(void);

#endif
