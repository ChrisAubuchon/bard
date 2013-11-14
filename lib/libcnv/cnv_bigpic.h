#ifndef _LIBCNV_CNV_BIGPIC_H
#define _LIBCNV_CNV_BIGPIC_H

/********************************/
/*				*/
/* Data structures		*/
/*				*/
/********************************/

typedef struct {
	btstring_t	*name;
	btstring_t	*path;
	uint8_t		type;
	uint8_t		isTimeAware;
} bt_bigpic_t;


/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

cnvList_t	*bigpicList_new(void);
void		bigpicList_toJson(cnvList_t *bpl, btstring_t *fname);

bt_bigpic_t	*bigpic_new(void);


#endif
