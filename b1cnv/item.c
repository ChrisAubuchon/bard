#include <b1lib.h>
#include <item.h>
#include <spell.h>
#include <cnv_spell.h>
#include <cnv_item.h>

/*#define DEBUG*/
#include <debug.h>

/********************************/
/*				*/
/* Local function prototypes	*/
/*				*/
/********************************/

static uint8_t getEffect(uint32_t i);
static uint32_t getValue(uint8_t v);

/********************************/
/*				*/
/* Local functions		*/
/*				*/
/********************************/

static uint8_t getEffect(uint32_t i)
{
	uint8_t rval = 0;

	rval = itemEffect[i] & 0x7f;
	if (rval >= 7)
		rval = 0;

	return rval;
}

static uint32_t getValue(uint8_t v)
{
	uint8_t i;
	uint32_t rval = 1;

	i = v & 7;
	while (i--)
		rval *= 10;

	return (rval * (v >> 3));
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

btstring_t *getItemName(uint32_t index)
{
	return bts_strcpy(itemName[index]);
}

void convertItems(void)
{
	int i;
	cnvList_t *il;
	btitem_t *b;

	il = itemList_new();
	for (i = 0; i < 127; i++) {

		b = btitem_new();

		b->name = bts_strcpy(itemName[i]);
		debug("Item name: %s\n", b->name->buf);
		b->macro = bts_strcpy(itemName[i]);
		str2macro(b->macro->buf);

		if (i < 22)
			b->shopCount = -1;
		else
			b->shopCount = 0;

		b->type = itemType[i + 1] & 0x0f;
		b->eclass = itemType[i + 1] & 0x0f;
		b->spAttack = itemType[i + 1] >> 4;
		b->canUse = (itemEffect[i] & 0x7f) > 15;

		b->ieffect = getEffect(i);

		b->hasCharges = b->canUse;
		b->maxCount = ((itemEffect[i] & 0x7f) >= 48) ? 1 : 0;

		b->acBonus = itemAttrib[i] & 0x0f;
		b->dmgBonus = itemAttrib[i] >> 4;
		if (b->type == 1) {
			b->ndice = NDICE(weapDice[i]);
			b->die = DIEVAL(weapDice[i]);
		}

		b->canEquip.warrior = IFBIT(itemEquipMask[i], 0x80, 1, 0);
		b->canEquip.paladin = IFBIT(itemEquipMask[i], 0x04, 1, 0);
		b->canEquip.rogue = IFBIT(itemEquipMask[i], 0x10, 1, 0);
		b->canEquip.bard = IFBIT(itemEquipMask[i], 0x08, 1, 0);
		b->canEquip.hunter = IFBIT(itemEquipMask[i], 0x02, 1, 0);
		b->canEquip.monk = IFBIT(itemEquipMask[i], 0x01, 1, 0);
		b->canEquip.conjurer = IFBIT(itemEquipMask[i], 0x40, 1, 0);
		b->canEquip.magician = IFBIT(itemEquipMask[i], 0x40, 1, 0);
		b->canEquip.sorcerer = IFBIT(itemEquipMask[i], 0x40, 1, 0);
		b->canEquip.wizard = IFBIT(itemEquipMask[i], 0x20, 1, 0);

		b->value = getValue(itemValue[i]);

		uint8_t spell = itemSpell[itemEffect[i] & 0x7f];

		debug("itemEffect: %d\n", itemEffect[i] & 0x7f);
		if ((itemEffect[i] & 0x7f) < 16) {
			b->use = USE_NONE;
		} else {
			getTargetting(itemSpell[itemEffect[i] & 0x7f], 
					&b->targetting);
			b->effect = getSpellEffect(spell);

			if ((spell == 90) || (spell == 91)) {
				b->use = USE_MAKELIGHT;
			} else if ((spell > 78) && (spell < 90)) {
				b->use = USE_BREATH;
			} else {
				b->use = USE_CASTSPELL;
			}
		}

		cnvList_add(il, b);
	}

	itemList_to_json(il, mkJsonPath("items.json"));
	shopList_to_json(il, mkJsonPath("garthinv.json"));
	cnvList_free(il);
}
