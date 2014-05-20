#ifndef _B3LIB_H
#define _B3LIB_H

#include <cnv_spell.h>
#include <cnv_param.h>
#include <bt_string.h>
#include <spell.h>

/********************************/
/*				*/
/* Global variables		*/
/*				*/
/********************************/

extern btstring_t	*inputDir;
extern btstring_t	*outputDir;


/********************************/
/*				*/
/* Structures			*/
/*				*/
/********************************/

typedef struct {
	uint8_t		hiflag;
	uint8_t		index;
} pic_t;

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define	IFBIT(x,y,true,false)	((x) & (y)) ? (true) : (false)

#define NDICE(x)	(((x) & 0x1f) + 1)
#define DIEVAL(x)	2 << (((x) >> 5) & 7)

#define mkImagePath(format, ...) \
	bts_sprintf("%s/bt3/images/"format, outputDir->buf, ## __VA_ARGS__)
#define mkImagePathRel(format, ...) \
	bts_sprintf("images/"format, ## __VA_ARGS__)
#define mkBardThreePath(format, ...) \
	bts_sprintf("%s/"format, inputDir->buf, ## __VA_ARGS__)
#define mkJsonPath(format, ...) \
	bts_sprintf("%s/bt3/json/"format, outputDir->buf, ## __VA_ARGS__)
#define mkCodePath(format, ...) \
	bts_sprintf("%s/bt3/code/"format, outputDir->buf, ## __VA_ARGS__)
#define mkFontPath(format, ...) \
	bts_sprintf("%s/bt3/fonts/"format, outputDir->buf, ## __VA_ARGS__)

/********************************/
/*				*/
/* Prototypes			*/
/*				*/
/********************************/

btstring_t	*getPicMacro(pic_t *p);
void		indexToPic(uint8_t index, pic_t *p);

void		getTargetting(uint32_t, btTargetting_t *);
btAction_t	*getSpellAction(uint32_t);
btAction_t	*cnvBreathAttack(breathAtt_t *, uint8_t);
btAction_t	*getFigurineAction(uint32_t);
btAction_t	*getWeaponAction(uint8_t);

/* item.c */
btstring_t	*getItemMacro(uint8_t index);

/* mon.c */
btstring_t	*getMonsterMacro(uint32_t index);
btstring_t	*getSummonMacro(uint32_t index);

/* data_ops.c */
btstring_t	*getSpellAbbr(uint8_t);
btstring_t	*getLevelMonster(uint8_t, uint8_t);


#if 0
/* common.c */
btstring_t *getDirectionMacro(uint8_t dir);

/* dun.c */
btstring_t *getLevelPath(uint8_t dun, uint8_t level);
btstring_t *getChangeLevelPath(uint8_t dun, uint8_t level, uint8_t direction);

/* item.c */
btstring_t *getLiquidMacro(uint8_t index);


/* picIndex.c */
uint8_t *getPicMacro(uint8_t pi);
uint8_t *getHiLoPicMacro(uint8_t hiflag, uint8_t index);

/* spell.c */
spellEffect_t *cnvBreathAttack(breathAtt_t *b, uint8_t range);
spellEffect_t *getSpellEffect(uint32_t spell);
spellEffect_t *noSpellEffect(void);
spellEffect_t *getFigurineEffect(uint32_t i);
spellEffect_t *getWeaponEffect(uint8_t i);
spellEffect_t *genericSpellEffect(uint8_t type, uint8_t flags);
btstring_t *getSpellAbbr(uint8_t spno);

/* trap.c */
btstring_t *getDunTrap(uint8_t level, uint8_t index);
btstring_t *getChestTrap(uint8_t level, uint8_t index);

#endif

#endif
