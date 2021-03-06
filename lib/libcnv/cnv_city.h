#ifndef __LIBCNV_CNV_CITY_H
#define __LIBCNV_CNV_CITY_H

#include <btlib.h>
#include <cnv_param.h>
#include <cnv_list.h>

/********************************/
/*				*/
/* Data structures		*/
/*				*/
/********************************/

typedef struct {
	cnvList_t	*items;
	cnvList_t	*monsters;
	uint8_t		poisonDmg;
	uint8_t		level;
} citydata_t;

typedef struct {
	btstring_t	*path;
	btstring_t 	*name;
	btstring_t	*title;
	btstring_t	*guildExitSquare;
	btstring_t	*guildExitDir;
	citydata_t	*day;
	citydata_t	*night;
	paramList_t	*params;
	cnvList_t	*sqs;
	cnvList_t	*bls;
} btcity_t;

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

cnvList_t	*cityList_new(void);
void		cityList_to_json(cnvList_t *cl, btstring_t *fname);

btcity_t	*btcity_new(btstring_t *name);
void		btcity_free(const void *vc);

void		citypath_new(btcity_t *btc, btstring_t *label,
				btstring_t *n, btstring_t *s, 
				btstring_t *e, btstring_t *w,
				btstring_t *o, btstring_t *l,
				btstring_t *f);
void		citybldg_new(btcity_t *btc, btstring_t *label, btstring_t *d,
				btstring_t *n, btstring_t *o);
void		city_addItem(btcity_t *btc, btstring_t *name, uint32_t weight);

void		btcity_to_json(btstring_t *fname, btcity_t *btc);

#endif
