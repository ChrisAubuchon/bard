#ifndef _DUN_DISASM_H
#define _DUN_DISASM_H

#include <cnv_code.h>
#include <cnv_dun.h>
#include <cnv_string.h>
#include <stdarg.h>

#define HASH_SIZE	521
#define OFFSETHASH_KEY	521

#define HASH_OFFSETDATA	1
#define HASH_STRING	2

#define STRING_PACKED	1
#define STRING_MASKED	2

typedef struct {
	uint16_t offset;
	bt_cnode_t *node;
} b3_offset_t;

typedef struct {
	bt_dllist_t *functions;
	bt_string_table_t *strtable;
} b3_code_t;

typedef struct {
  uint8_t ifBlock;
  uint8_t clearBreak;
  uint8_t *args;
} mapfunc_t;


/********************************/
/*				*/
/* mfunc macros			*/
/*				*/
/********************************/

#define mfunc_none		{ 0, 0, "" }
#define mfunc_downStairs	{ 0, 0, "" }
#define mfunc_upStairs		{ 0, 0, "" }
#define sub_1942b		{ 0, 0, "z" }
#define mfunc_teleport		{ 0, 0, "bbb" }
#define mfunc_battle		{ 1, 0, "B" }
#define mfunc_getCh		{ 0, 0, "" }
#define mfunc_printStringWClear	{ 0, 0, "s" }
#define mfunc_ffcode		{ 0, 0, "w" }
#define mfunc_drawBigpic	{ 0, 0, "b" }
#define mfunc_setTitle		{ 0, 0, "S" }
#define mfunc_getCh		{ 0, 0, "" }
#define mfunc_clearTextWindow	{ 0, 0, "" }
#define mfunc_ifFlag		{ 1, 0, "b" }
#define mfunc_ifNotFlag		{ 1, 0, "b" }
#define mfunc_makeDoor		{ 0, 0, "bbb" }
#define mfunc_setFlag		{ 0, 0, "b" }
#define mfunc_clearFlag		{ 0, 0, "b" }
#define mfunc_ifCurSpellEQ	{ 1, 0, "b" }
#define mfunc_setMapRval	{ 0, 0, "" }
#define mfunc_printString	{ 0, 0, "s" }
#define mfunc_doNothing		{ 0, 0, "" }
#define mfunc_ifContainerLiquid	{ 1, 0, "b" }
#define mfunc_getItem		{ 1, 0, "bbb" }
#define mfunc_ifHasItem		{ 1, 0, "b" }
#define mfunc_ifNotHasItem	{ 1, 0, "b" }
#define mfunc_ifOnNewSquare	{ 1, 0, "" }
#define mfunc_ifYesNo		{ 1, 0, "" }
#define mfunc_goto		{ 0, 1, "o" }
#define mfunc_battleNoCry	{ 1, 0, "B" }
#define mfunc_setOnNewSquare	{ 0, 0, "" }
#define mfunc_turnAround	{ 0, 0, "" }
#define mfunc_removeItem	{ 0, 0, "b" }
#define mfunc_incRegister	{ 0, 0, "b" }
#define mfunc_decRegister	{ 0, 0, "b" }
#define mfunc_ifRegZero		{ 1, 0, "b" }
#define mfunc_ifRegister	{ 1, 0, "b" }
#define mfunc_drainHP		{ 0, 0, "w" }
#define mfunc_ifInBox		{ 1, 0, "bbbb" }
#define mfunc_setContainerLiquid	{ 0, 0, "b" }
#define mfunc_addToInv		{ 0, 0, "b" }
#define mfunc_subFromInv	{ 0, 0, "b" }
#define mfunc_addToRegister	{ 0, 0, "bw" }
#define mfunc_subFromRegister	{ 0, 0, "bw" }
#define mfunc_setDirFacing	{ 0, 0, "b" }
#define mfunc_getStrFromUser	{ 0, 0, "" }
#define mfunc_ifStrEq		{ 1, 0, "S" }
#define mfunc_setRegFromBuf	{ 0, 0, "b" }
#define mfunc_getCharacter	{ 1, 0, "" }
#define mfunc_ifGiveGold	{ 1, 0, "b" }
#define mfunc_ifRegLT		{ 1, 0, "bw" }
#define mfunc_ifRegEQ		{ 1, 0, "bw" }
#define mfunc_ifRegGE		{ 1, 0, "bw" }
#define mfunc_learnSpell	{ 0, 0, "b" }
#define mfunc_setRegister	{ 0, 0, "bw" }
#define mfunc_chHasItemNo	{ 1, 0, "b" }
#define mfunc_packInvMaybe	{ 0, 0, "" }
#define mfunc_addMonToParty	{ 1, 0, "b" }
#define mfunc_ifMonInParty	{ 1, 0, "S" }
#define mfunc_clrPrtStrOffset	{ 0, 0, "M" }
#define mfunc_ifIsNight		{ 1, 0, "" }
#define mfunc_rmMonFromParty	{ 0, 0, "" }
#define mfunc_setChronQuestFlag	{ 0, 0, "b" }
#define mfunc_isWildSqFlagSet	{ 1, 0, "b" }
#define mfunc_wildSetSqFlag	{ 0, 0, "b" }
#define mfunc_ifIsClass		{ 1, 0, "b" }
#define mfunc_printStrAtOffset	{ 0, 0, "M" }
#define mfunc_clearAndTeleport	{ 0, 0, "bbb" }



/********************************/
/*				*/
/* Public Interface		*/
/*				*/
/********************************/

void disasm_code(dun_t *d, uint8_t dunno);
void disasm_destroy(void);
dunLevel_t *getCurrentLevel(void);

#endif
