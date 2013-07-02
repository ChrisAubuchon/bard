#ifndef _CNV_H
#define _CNV_H

/*
#include <bt/item.h>
#include <bt/mon.h>
*/
#include <cnv_gfx.h>
#include <cnv_dun.h>

/********************************/
/* 				*/
/* Macros			*/
/* 				*/
/********************************/

/********************************/
/* 				*/
/* Function Prototypes		*/
/* 				*/
/********************************/

/* item.c */
/*
void printItemMacro(btitem_t *i);
void printItemXML(btitem_t *i);
*/

/* mon.c */
/*
void printMonXML(btmon_t *m);
void printMonMacro(btmon_t *m);
*/

/* spell.c */
uint8_t *getSpecialAttack(uint8_t sp);
uint8_t *getBreathAttack(uint8_t br);
uint8_t *getSpellEffectString(uint8_t type);

void clearSpell(spell_t *sp, uint8_t type);
void printSpellEffectXML(spell_t *sp, uint8_t type, uint8_t indent);
void printSpellEffectMacro(spell_t *sp, uint8_t type);
void printRepelXML(repel_t r, uint8_t indent);
void printElemXML(elements_t e, uint8_t indent);
void printRepelMacro(repel_t r);
void printElemMacro(elements_t e);
void printSpellXML(mageSpell_t *m);

/* anim_gfx.c */
gfx_animimg *gfxanimimg_new(void);
gfx_anim *gfxanim_new(uint8_t ncells);
gfx_slides *gfxslides_new(uint8_t nslides);
void gfxanimimg_free(gfx_animimg *ai);
void gfxanim_free(gfx_anim *ga);
void gfxslides_free(gfx_slides *sl);
void outputAnimimgXML(uint8_t indent, gfx_animimg *ai);
void outputAnimXML(uint8_t indent, gfx_anim *an);
void outputSlidesXML(uint8_t indent, gfx_slides *sl);

/* bigpic_gfx.c */
gfx_bpdata *gfxbpdata_new(uint8_t nloops);
gfx_bigpic *gfxbigpic_new(uint8_t npics);
void gfxbpdata_free(gfx_bpdata *bd);
void gfxbigpic_free(gfx_bigpic *bp);
void outputBigpicXML(uint8_t indent, gfx_bigpic *bp);

/* tile_gfx.c */
void outputTilesetXML(uint8_t indent, gfx_tileset *tile);
void outputQuadrantXML(uint8_t indent, gfx_quad *quad);

gfx_tileset *gfxtile_new(uint8_t nquads);
gfx_quad *gfxquad_new(uint8_t nfacets);
void gfxquad_free(gfx_quad *qd);
void gfxtile_free(gfx_tileset *tile);

btstring_t *getQuadString(uint8_t quad, uint8_t qnim);
btstring_t *getTilesetTypeString(uint8_t type);

/* gfx_imglist.c */
void outputImglistXML(uint8_t indent, gfx_imglist *img);
void outputImageXML(uint8_t indent, gfx_image *img);
void gfximglist_free(gfx_imglist *il);
gfx_imglist *gfximglist_new(uint8_t nimgs);
void gfximage_free(gfx_image *img);
gfx_image *gfximage_new(void);

/* dun.c */
dun_t *dun_new(uint8_t nlevs);
dunLevel_t *dunLevel_new(uint8_t width, uint8_t height);
dunSquare_t *dunSquare_new(void);
void dun_free(dun_t *d);
void dunLevel_free(dunLevel_t *dl);
void dunSquare_free(dunSquare_t *ds);
void outputDungeonXML(uint8_t indent, dun_t *d);


#endif
