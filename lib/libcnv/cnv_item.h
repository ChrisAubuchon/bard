#ifndef _LIBCNV_ITEM_H
#define _LIBCNV_ITEM_H

#include <cnvlib.h>
#include <cnv_spell.h>

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define USE_NONE        0
#define USE_CASTSPELL   1
#define USE_MAKELIGHT   2
#define USE_FIGURINE    3
#define USE_ATEIT       4
#define USE_CASTWEAPON  5
#define USE_BREATH      6
#define USE_REENERGIZE  7
#define USE_DRINK       8
#define USE_SHOOTARROW  9
#define USE_THROWSPEAR  10
#define USE_THROWAXE    11
#define USE_HURLSHURI   12
#define USE_THROWBOOM   13
#define USE_HURLHAMMER  14
#define USE_THROWBLADE  15

/********************************/
/*				*/
/* Item structure		*/
/*				*/
/********************************/

typedef struct {
	uint32_t	weight;
	btstring_t	*name;
} levelItem_t;

typedef struct {
  btstring_t	*name;		/* Item name                    */
  btstring_t	*macro;		/* Item macro                   */

  uint8_t	type;		/* Item type                    */
  uint8_t	eclass;		/* Equipment class              */
  uint8_t	ieffect;	/* Item effect                  */
  uint8_t	targetted;	/* Does the object require a target     */
  uint8_t	canUse;		/* Flag for an item that can be used    */
  uint8_t	hasCharges;	/* Flag for an item with a use counter  */
  uint8_t	rndCharges;	/* If set, randomly determine the number of charges     */
				/* an item has                                          */
  uint8_t	combat;		/* Can use in combat            */
  uint8_t	noncombat;	/* Can be used out of combat    */

  uint8_t	dmgBonus;	/* Damage bonus                 */
  uint8_t	acBonus;	/* Armor class bonus            */

  uint8_t	ndice;		/* Number of dice for damage    */
  uint8_t	die;		/* Dice value                   */
  uint8_t	spAttack;	/* Special attack type          */

  uint8_t	maxCount;	/* Max number of charges (1 - maxCount) */
				/* If this is 0 and hasCharges is 1     */
				/* there is a 1 in 64 chance that the   */
				/* item will be consumed after use.     */
				/* This is to match BTI's use behavior  */

  uint32_t value;               /* Item price                   */
  int8_t	shopCount;	/* Initial shop count		*/

  struct {                      /* Bits for classes that the    */
    uint8_t	warrior;        /* item can be equipped by      */
    uint8_t	wizard;
    uint8_t	sorcerer;
    uint8_t	conjurer;
    uint8_t	magician;
    uint8_t	rogue;
    uint8_t	bard;
    uint8_t	paladin;
    uint8_t	hunter;
    uint8_t	monk;
    uint8_t	archmage;
    uint8_t	chronomancer;
    uint8_t	geomancer;
  } canEquip;

	btTargetting_t	targetting;
	uint8_t		use;
	btEffect_t	*effect;
} btitem_t;

/********************************/
/*				*/
/* Function prototypes		*/
/*				*/
/********************************/

cnvList_t	*itemList_new(void);
void		itemList_to_json(cnvList_t *il, btstring_t *fname);
void		shopList_to_json(cnvList_t *sl, btstring_t *fname);

levelItem_t	*levelItem_new(btstring_t *name, uint32_t weight);
void		levelItem_free(const void *vli);
json_t		*levelItem_toJson(const void *vli);
btstring_t	*levelItem_toName(const void *vli);

btitem_t *btitem_new(void);

#endif
