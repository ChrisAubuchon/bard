#ifndef _LIBCNV_CNV_CLASSES_H
#define _LIBCNV_CNV_CLASSES_H

/********************************/
/*				*/
/* btClass structure		*/
/*				*/
/********************************/

typedef struct {
	btstring_t	*name;
	btstring_t	*pic;
	uint32_t	hpDice;
	uint32_t	meleeBonus;
	cnvList_t	*xpReq;
} btClass_t;

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

cnvList_t	*classList_new(void);
void		classList_toJson(cnvList_t *cl, btstring_t *fname);

btClass_t	*btClass_new(void);

#endif
