#ifndef _B1LIB_H
#define _B1LIB_H

#include <cnv_spell.h>
#include <cnv_action.h>
#include <bt_string.h>

#define IFBIT(x,y,true,false)	((x) & (y)) ? (true) : (false)

#define DIEVAL(x)       4 << ((x) >> 5)
#define NDICE(x)        (((x) & 0x1f) + 1)

#define mkImagePath(format, ...)	\
	bts_sprintf("%s/bt1/images/"format, DATADIR, ## __VA_ARGS__)
#define mkImagePathRel(format, ...)	\
	bts_sprintf("images/"format, ## __VA_ARGS__)

#define mkFontPath(format, ...)		\
	bts_sprintf("%s/bt1/fonts/"format, DATADIR, ## __VA_ARGS__)

#define mkJsonPath(format, ...)		\
	bts_sprintf("%s/bt1/json/"format, DATADIR, ## __VA_ARGS__)

#define mkJsonPathRel(format, ...)	\
	bts_sprintf("json/"format, ## __VA_ARGS__)

#define mkLuaPath(format, ...)		\
	bts_sprintf("%s/bt1/lua/"format, DATADIR, ## __VA_ARGS__)

#define mkBardOnePath(format, ...)		\
	bts_sprintf("%s/"format, BASEDIR, ## __VA_ARGS__)

/* class.c */
void		convertClasses(void);

/* spell.c */
void		convertSpells(void);
btAction_t	*getSpellAction(uint32_t spell);
void		getTargetting(uint32_t index, btTargetting_t *bt);

/* gfx.c */
void convertGFX(void);

/* gfx_bigpic.c */
void outputBigpic(uint8_t indent);

/* gfx_city.c */
void outputCitypics(uint8_t indent);
void flipRight(btstring_t *r, uint8_t height, uint8_t width);

/* gfx_dun.c */
void outputDunpics(uint8_t indent);

/* item.c */
void convertItems(void);
btstring_t *getItemName(uint32_t index);

/* mon.c */
void convertMonsters(void);
btstring_t *getMonsterName(uint32_t index);
uint8_t *getMonMacro(uint32_t index);

/* cnv.c */
void cnv_printSpellHeader(void);
void cnv_printSpellFooter(void);
void cnv_printMonHeader(void);
void cnv_printMonFooter(void);

/* picIndex.c */
void printPicMacro(uint8_t pi);
void picHeader(void);
btstring_t *getPicMacro(uint8_t pi);
void outputPicXML(void);

/* common.c */
void getSavingThrow(uint8_t type, uint8_t *lo, uint8_t *hi);

/* trap.c */
void outputTraps(void);
btstring_t *getDunTrap(uint8_t level, uint8_t index);
btstring_t *getChestTrap(uint8_t level, uint8_t index);

#endif


