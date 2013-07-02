#ifndef _LIBCNV_CNV_RACE_H
#define _LIBCNV_CNV_RACE_H


/********************************/
/*				*/
/* btRace_t structure		*/
/*				*/
/********************************/

typedef struct {
	btstring_t	*name;
	uint32_t	base_st;
	uint32_t	base_dx;
	uint32_t	base_iq;
	uint32_t	base_cn;
	uint32_t	base_lk;
	cnvList_t	*startingClasses;
} btRace_t;

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

cnvList_t	*raceList_new(void);
void		raceList_toJson(cnvList_t *rl, btstring_t *fname);

btRace_t	*btRace_new(void);

#endif
