#include <b3lib.h>
#include <item.h>
#include <cnv_item.h>

#define DEBUG
#include <debug.h>

/********************************/
/*				*/
/* Local Function Prototypes	*/
/*				*/
/********************************/

static uint8_t getWeapIndex(uint8_t w);
static uint8_t getFigIndex(uint8_t f);

/********************************/
/*				*/
/* Static Data			*/
/*				*/
/********************************/

static uint8_t figEffInd[] = { 
27, 38, 56, 61, 77, 98, 99, 190, 204, 0
};

static uint8_t figEff[] = {
13, 14, 15, 1, 16, 17, 18, 19, 20, 0
};

/********************************/
/*				*/
/* Local Functions		*/
/*				*/
/********************************/

static uint8_t getWeapIndex(uint8_t w)
{
  uint8_t i;

  for (i = 0; i < 32; i++) {
    if (weapEffInd[i] == w)
      return i;
  }

  return 0xff;
}

static uint8_t getFigIndex(uint8_t f)
{
  uint8_t i;

  for (i = 0; i < 10; i++) {
    if (figEffInd[i] == f)
      return figEff[i];
  }

  return 0xff;
}

/********************************/
/*				*/
/* Public Functions		*/
/*				*/
/********************************/

btstring_t *getItemMacro(uint8_t index)
{
  btstring_t *rval;

  rval = bts_strcpy(itemName[index]);
  str2macro(rval->buf);

  return rval;
}

btstring_t *getLiquidMacro(uint8_t index)
{
	btstring_t *rval;

	rval = bts_strcpy(liquidNameList[index]);
	str2macro(rval->buf);

	return rval;
}

void convertItems(void)
{
  int i;
  uint32_t count = 0;
  uint8_t spell;

  cnv_printItemHeader();

  for (i = 0; i < 256; i++) {
    btitem_t *b;

    if (itemName[i][0] == 'x')
      continue;

    b = btitem_new();

    b->name = bts_strcpy(itemName[i]);
    b->macro = bts_strcpy(itemName[i]);
    str2macro(b->macro->buf);

    /* Item Type */
    b->type = itemType[i] & 0x0f;

    /* Item equippable-by flags */
    b->canEquip.warrior = IFBIT(itemEquipMask[i], 0x80, 1, 0);
    b->canEquip.wizard = IFBIT(itemEquipMask[i], 0x40, 1, 0);
    b->canEquip.sorcerer = IFBIT(itemEquipMask[i], 0x40, 1, 0);
    b->canEquip.conjurer = IFBIT(itemEquipMask[i], 0x40, 1, 0);
    b->canEquip.magician = IFBIT(itemEquipMask[i], 0x40, 1, 0);
    b->canEquip.rogue = IFBIT(itemEquipMask[i], 0x10, 1, 0);
    b->canEquip.bard = IFBIT(itemEquipMask[i], 0x08, 1, 0);
    b->canEquip.paladin = IFBIT(itemEquipMask[i], 0x04, 1, 0);
    b->canEquip.hunter = IFBIT(itemEquipMask[i], 0x02, 1, 0);
    b->canEquip.monk = IFBIT(itemEquipMask[i], 0x01, 1, 0);
    b->canEquip.archmage = IFBIT(itemEquipMask[i], 0x20, 1, 0);
    b->canEquip.chronomancer = IFBIT(itemEquipMask[i], 0x60, 1, 0);
    b->canEquip.geomancer = IFBIT(itemEquipMask[i], 0xe0, 1, 0);

    b->type = itemType[i] & 0x0f;
    b->eclass = itemType[i] & 0x0f;
    b->effect = itemEffectList[i];
    b->spAttack = itemType[i] >> 4;

    b->dmgBonus = itemArmWepBonus[i] >> 4;
    b->acBonus  = itemArmWepBonus[i] & 0x0f;
    b->canUse = (itemSpell[i] != 0xff);
    b->hasCharges = (itemCountMax[i] != 0xff);
    b->rndCharges = ((itemCountMax[i] != 0xff) && (itemCountMax[i] > 1));

    spell = itemSpell[i];

    if (b->type == 1) {
      b->ndice = NDICE(itemDmgDice[i]);
      b->die   = DIEVAL(itemDmgDice[i]);
    }

    if (spell == 255) {
      b->targetted = 0;
    } else {
      b->targetted = ((spellAttr[itemSpell[i]] & 7) < 4);
    }
    b->combat = b->canUse;
    b->noncombat = b->canUse;

    if (spell == 255) {
      b->spell = noSpellEffect();
    } else {
      if (spell < 125) {
	b->spell = getSpellEffect(spell);
	b->spell->subtype = USE_CASTSPELL;
      } else if ((spell == 126) || (spell == 127)) {
	b->spell = getSpellEffect(spell);
	b->spell->subtype = USE_MAKELIGHT;
      } else if (spell == 128) {
	b->spell = getSpellEffect(20);
	b->spell->subtype = USE_ATEIT;
      } else if (spell == 129) {
	b->spell = getSpellEffect(20);
	((eff_Heal *)(b->spell->effect))->quench_bard = 1;
	b->spell->subtype = USE_DRINK;
      } else if (spell == 130) {
	b->spell = getSpellEffect(130);
	b->spell->subtype = USE_NONE;
      } else if (spell == 131) {
	b->spell = getWeaponEffect(getWeapIndex(i));
	b->spell->subtype = USE_CASTWEAPON;
      } else if (spell == 132) {
	b->spell = getFigurineEffect(getFigIndex(i));
	b->spell->subtype = USE_FIGURINE;
      } else if (spell == 133) {
	b->spell = getWeaponEffect(getWeapIndex(i));
	b->spell->subtype = USE_BREATH;
      } else if (spell == 134) {
	b->spell = getSpellEffect(spell);
	b->spell->subtype = USE_REENERGIZE;
      }

      b->spell->type = ACT_USE;
    }

    printItemXML(2, b);

    btitem_free(b);

  }

  cnv_printItemFooter();
}
