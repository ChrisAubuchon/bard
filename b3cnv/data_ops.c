#include <b3lib.h>
#include <spell.h>
#include <item.h>
#include <mon.h>
#include <dun.h>

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

/*
 * getSpellAbbr()
 */
btstring_t *getSpellAbbr(uint8_t spell)
{
	return bts_strcpy(spName[spell].abbr);
}

/*
 * getItemMacro()
 */
btstring_t *getItemMacro(uint8_t index)
{
	btstring_t	*rval;

	rval = bts_strcpy(itemName[index]);
	str2macro(rval->buf);

	return rval;
}

/*
 * getLiquid()
 */
btstring_t *getLiquid(uint8_t index)
{
	btstring_t	*rval;

	rval = bts_strcpy(liquidNameList[index]);
	str2macro(rval->buf);

	return rval;
}

/*
 * getLevelMonster()
 */
btstring_t *getLevelMonster(uint8_t level, uint8_t index)
{
	uint16_t	monster;

	monster = dun_monIndexList[level].mon_lo + index;
	
	return bts_strcpy(monMacro[monster]);
}
