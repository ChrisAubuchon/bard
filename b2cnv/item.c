#include <b2lib.h>
#include <item.h>
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
	if (rval >= 0x0d)
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
	cnvList_t	*il;
	btitem_t	*b;
	uint32_t	i;

	il = itemList_new();
	for (i = 0; i < 127; i++) {
		b = btitem_new();

		b->name = bts_strcpy(itemName[i]);
		debug("Item: %s\n", b->name->buf);
		b->macro = bts_strcpy(itemName[i]);
		str2macro(b->macro->buf);

		b->type = itemType[i] & 0x0f;
		b->eclass = itemEClass[i] & 0x0f;
		b->spAttack = itemType[i] >> 4;
		b->canUse = (itemSpell[i] != 0);

		b->ieffect = getEffect(i);

		b->hasCharges = (itemCount[i] != 0xff);
		b->maxCount = (itemCount[i] == 0xff) ? 0 : itemCount[i];

		b->combat = IFBIT(itemEffect[i], 0x80, 1, 0);

		b->dmgBonus = itemDmgAc[i] >> 4;
		b->acBonus = itemDmgAc[i] & 0x0f;

		if (b->type == 1) {
			b->ndice = NDICE(itemDmgDice[i]);
			b->die = DIEVAL(itemDmgDice[i]);
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
		b->canEquip.archmage = IFBIT(itemEquipMask[i], 0x20, 1, 0);

#if 0
		if (itemSpell[i] == 0) {
			b->targetted = 0;
		} else {
			b->targetted = ((spellAttr[itemSpell[i]] & 7) < 4);
		}
#endif

		b->value = getValue(itemValue[i]);

		uint8_t spell = itemSpell[i];
		if (spell) {
			getTargetting(itemSpell[i], &b->targetting);
			if (spell > 111) {
				debug("spell = %d\n", spell);
				b->action = getSummonEffect(spell);
			} else {
				b->action = getSpellAction(spell);
			}

			if (spell < 79) {
				b->use = USE_CASTSPELL;
			} else if (spell < 96) {
				b->use = USE_BREATH;
			} else if ((spell == 96) || (spell == 97)) {
				b->use = USE_MAKELIGHT;
			} else if (spell < 112) {
				bteAttack_t	*ba;

				ba = btEffect_attack(b->action->effect);
				ba->fireString = useWeap[spell - 98];
			} else {
				b->use = USE_FIGURINE;
			}
		}
#if 0
		if (spell == 0) {
			b->spell = noSpellEffect();
		} else if (spell < 79) {
			b->spell = getSpellEffect(spell);
			b->spell->subtype = USE_CASTSPELL;
			b->spell->type = ACT_USE;
		} else if (spell < 96) {
			b->spell = getSpellEffect(spell);
			b->spell->subtype = USE_BREATH;
			b->spell->type = ACT_USE;
		} else if ((spell == 96) || (spell == 97)) {
			b->spell = getSpellEffect(spell);
			b->spell->subtype = useWeap[spell - 96];
			b->spell->type = ACT_USE;
		} else if (spell < 112) {
			b->spell = getSpellEffect(spell);
			b->spell->subtype = useWeap[spell - 96];
			b->spell->type = ACT_USE;
		} else if (spell > 111) {
			b->spell = getSummonEffect(spell);
			b->spell->subtype = USE_FIGURINE;
			b->spell->type = ACT_USE;
		}
#endif

		cnvList_add(il, b);

	}

	itemList_to_json(il, mkJsonPath("items.json"));
	shopList_to_json(il, mkJsonPath("garthinv.json"));
	cnvList_free(il);
}
