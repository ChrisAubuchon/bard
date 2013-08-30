#ifndef _B2LIB_H
#define _B2LIB_H

#include <cnv_spell.h>
#include <bt_string.h>


/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define DIEVAL(x)       4 << ((x) >> 5)
#define NDICE(x)        (((x) & 0x1f) + 1)

#define mkImagePath(format, ...)	\
	bts_sprintf("%s/bt2/images/"format, DATADIR, ## __VA_ARGS__)
#define mkImagePathRel(format, ...)	\
	bts_sprintf("images/"format, ## __VA_ARGS__)
#define mkFontPath(format, ...)	\
	bts_sprintf("%s/bt2/fonts/"format, DATADIR, ## __VA_ARGS__)
#define mkJsonPath(format, ...)	\
	bts_sprintf("%s/bt2/json/"format, DATADIR, ## __VA_ARGS__)
#define mkJsonPathRel(format, ...)	\
	bts_sprintf("json/"format, ## __VA_ARGS__)
#define mkBardTwoPath(format, ...)	\
	bts_sprintf("%s/"format, BASEDIR, ## __VA_ARGS__)

/********************************/
/*				*/
/* Function prototypes		*/
/*				*/
/********************************/

/* common.c */
void printSpecialAttack(uint8_t sp);
uint8_t *breathAttackEffect(uint8_t br);

/* cnv.c */
void cnv_printSpellHeader(void);
void cnv_printSpellFooter(void);
void cnv_printMonHeader(void);
void cnv_printMonFooter(void);

/* items.c */
void convertItems(void);

/* monster.c */
void convertMonsters(void);
void monsterHeader(void);
void mon2summon(uint32_t index);
btstring_t *getSummonMacro(uint32_t index);

/* spell.c */
void spellHeader(void);
void convertSpells(void);
#if 0
spellEffect_t *getSummonEffect(uint32_t spell);
spellEffect_t *getSpellEffect(uint32_t spell);
spellEffect_t *noSpellEffect(void);
#endif

void convertItems(void);

/* picIndex.c */
void printPicMacro(uint8_t pi);
void printPicMacros(void);
btstring_t *getPicMacro(uint8_t pi);
void outputPicXML(void);


/* gfx functions */
void flipRight(btstring_t *r, uint8_t height, uint8_t width);
void convertGFX(void);
void outputBigpic(void);
void outputCitypics(void);
void outputDunpics(void);

/* trap.c */
void convertTraps(void);
btstring_t *getDunTrap(uint8_t level, uint8_t index);
btstring_t *getChestTrap(uint8_t level, uint8_t index);

#endif

