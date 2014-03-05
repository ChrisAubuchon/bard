#include <b3lib.h>
#include <dun.h>
#include <cnv_dun.h>
#include <cnv_code.h>
#include <dun_disasm.h>
#include <assert.h>
#include <bt_gds.h>
#include <cnv_string.h>

#define DEBUG
#include <debug.h>

/********************************/
/*				*/
/* Local Macros			*/
/*				*/
/********************************/

#define off_to_file(off)	((off) + disasm->base_offset)
#define off_from_file(off)	((off) - disasm->base_offset)

typedef struct {
	uint16_t base_offset;
	uint8_t levno;
	uint8_t dunno;

	btstring_t *code;
	btstring_t *bad_offset;
	dunLevel_t *curLevP;
	dun_t *curDunP;
} b3_disasm_t;

/********************************/
/*				*/
/* Global Variables		*/
/*				*/
/********************************/

static b3_code_t *decomp;

static bt_hash_table_t *offset_hash;

static bt_stack_t *unresolved_offsets;
static bt_stack_t *jump_stack;

static b3_disasm_t *disasm;

/* String extraction variables */
static int8_t _str_bitsLeft;
static uint8_t _str_curByte;
static uint8_t _str_capFlag;

/********************************/
/*				*/
/* Local function prototypes	*/
/*				*/
/********************************/

static void convertB3code(void);

static void add_unresolved_offset(uint16_t offset, bt_cnode_t *node);
static void resolve_references(void);

static bt_array_t *read_args(uint8_t op, uint16_t *offset);

static bt_cnode_t *do_disasm(uint16_t *offset);
static bt_cnode_t *do_disasm_opcode(uint8_t op, uint16_t *offset);
static b3_code_t *tree_disasm_level(void);
static bt_cnode_t *do_convertOpcode(uint16_t *offset, uint8_t opcode, bt_array_t *args);
static bt_cnode_t *check_jump(uint16_t offset);
static void skip_finished_offsets(uint16_t *offset);

/* Offset hash table functions */
static void b3_offset_free(void *bod);
static uint32_t b3_offset_get_hash_key(void *bod);
static uint8_t b3_offset_compare(void *src, void *cmp);
static void b3_offset_dump(void *bod);
static bt_cnode_t *b3_offset_search(uint16_t offset);
static void b3_offset_tree_free(void *bod);

static b3data_t *searchForSquare(uint16_t offset);

static bt_cnode_t *do_disasm_opcode(uint8_t op, uint16_t *offset);

static bt_cnode_t *cnvB3teleport(bt_array_t *args);
static bt_cnode_t *cnvB3stairs(uint16_t *offset, uint8_t dirFlag);
static bt_cnode_t *cnvB3misc(bt_array_t *args);
static bt_cnode_t *cnvB3battle(bt_array_t *args, uint8_t cryFlag);
static bt_cnode_t *cnvB3printString(bt_array_t *args, uint8_t clearFlag);
static bt_cnode_t *cnvB3action(bt_array_t *args);

/********************************/
/*				*/
/* Static data			*/
/*				*/
/********************************/

static uint8_t flagMaskList[] = {
	0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe
};

static uint8_t _str_Loalphabet[] = {
	'\0', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
	'k', 'l', 'm', 'n', 'o', 'p', 'r', 's', 't', 'u', 'v',
	'w', 'y', '.', '"', '\'', ',', '!', '\n'
};

static uint8_t _str_Hialphabet[] = {
	'j', 'q', 'x', 'z', '0', '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
	'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
	'U', 'V', 'W', 'X', 'Y', 'Z', '(', ')', '/', '\\', '#',
	'*', '?', '<', '>', ':', ';', '-', '%'
};


static uint8_t alphaFlags[] = {
	32, 32, 32, 32, 32, 32, 32, 32, 32, 40, 40, 40, 40, 40, 32, 32,
	32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	72, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
	132, 132, 132, 132, 132, 132, 132, 132, 132, 132, 16, 16, 16, 16, 16, 16,
	16, 129, 129, 129, 129, 129, 129, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 16, 16, 16, 16, 16,
	16, 130, 130, 130, 130, 130, 130, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 16, 16, 16, 16, 32,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

mapfunc_t opFuncs[] = {
/* 0 */ mfunc_downStairs, mfunc_upStairs, sub_1942b, mfunc_teleport, mfunc_battle,
/* 5 */ mfunc_printStringWClear, mfunc_ffcode, mfunc_drawBigpic, mfunc_setTitle, mfunc_getCh,
/* a */ mfunc_clearTextWindow, mfunc_ifFlag, mfunc_ifNotFlag, mfunc_makeDoor, mfunc_setFlag,
/* f */ mfunc_clearFlag, mfunc_ifCurSpellEQ, mfunc_setMapRval, mfunc_printString, mfunc_doNothing,
/* 14*/ mfunc_ifContainerLiquid, mfunc_getItem, mfunc_ifHasItem, mfunc_ifNotHasItem, mfunc_ifOnNewSquare,
/* 19*/ mfunc_ifYesNo, mfunc_goto, mfunc_battleNoCry, mfunc_setOnNewSquare, mfunc_turnAround,
/* 1e*/ mfunc_removeItem, mfunc_incRegister, mfunc_decRegister, mfunc_ifRegZero, mfunc_ifRegister,
/* 23*/ mfunc_drainHP, mfunc_ifInBox, mfunc_setContainerLiquid, mfunc_addToInv, mfunc_subFromInv,
/* 28*/ mfunc_addToRegister, mfunc_subFromRegister, mfunc_setDirFacing, mfunc_getStrFromUser, mfunc_ifStrEq,
/* 2d*/ mfunc_setRegFromBuf, mfunc_getCharacter, mfunc_ifGiveGold, mfunc_none, mfunc_ifRegLT,
/* 32*/ mfunc_ifRegEQ, mfunc_ifRegGE, mfunc_learnSpell, mfunc_setRegister, mfunc_chHasItemNo,
/* 37*/ mfunc_packInvMaybe, mfunc_addMonToParty, mfunc_ifMonInParty, mfunc_clrPrtStrOffset, mfunc_ifIsNight,
/* 3c*/ mfunc_rmMonFromParty, mfunc_none, mfunc_none, mfunc_setChronQuestFlag, mfunc_none,
/* 41*/ mfunc_none, mfunc_isWildSqFlagSet, mfunc_wildSetSqFlag, mfunc_ifIsClass, mfunc_printStrAtOffset,
/* 46*/ mfunc_clearAndTeleport
};


/********************************/
/*				*/
/* Local functions		*/
/*				*/
/********************************/

static b3_code_t *tree_disasm_level(void)
{
	uint16_t offset = 0;

	disasm->code = disasm->curLevP->codeBuffer;
	disasm->bad_offset = bts_new(disasm->code->size);
	disasm->base_offset = duns[disasm->dunno].files[disasm->levno].code_offset;

	decomp = (b3_code_t *)xmalloc(sizeof(b3_code_t));
	decomp->strtable = bt_string_table_new();
	decomp->functions = bt_dllist_new(NULL, b3_offset_tree_free);

	/* Create linked list for unresolved offsets */
	unresolved_offsets = bt_stack_new();
	jump_stack = bt_stack_new();


	while (offset < disasm->code->size) {
		b3_offset_t *root;

		root = (b3_offset_t *)xmalloc(sizeof(b3_offset_t));
		root->offset = offset;

		debug("Entry point = 0x%x\n", off_to_file(offset));
		offset_hash = bt_hash_new(HASH_SIZE, b3_offset_get_hash_key, b3_offset_compare, b3_offset_free);

		root->node = do_disasm(&offset);
		bt_dllist_append(decomp->functions, root);

		resolve_references();

		outputCodeTreeXML(root->node, 0);
		printf("\n------------------------------------------\n\n");

		skip_finished_offsets(&offset);

		bt_hash_free(offset_hash);
	}

	bt_stack_free(jump_stack);
	bt_stack_free(unresolved_offsets);
	bts_free(disasm->bad_offset);

	return decomp;
}

static bt_cnode_t *do_disasm(uint16_t *offset)
{
	b3_offset_t *offnode;
	bt_cnode_t *root = NULL;
	bt_cnode_t *curnode;
	uint8_t breakAfterFunc;
	uint8_t op;

	/*
	 * Check if this offset has already been disassembled. If it has,
	 * return an OP_JUMP node.
	 */
	root = check_jump(off_to_file(*offset));
	if (root != NULL)
		return root;

	for (;;) {
		skip_finished_offsets(offset);

		if (*offset >= disasm->code->size)
			return root;

		offnode = (b3_offset_t *)xmalloc(sizeof(b3_offset_t));
		offnode->offset = *offset + disasm->base_offset;

		disasm->bad_offset->buf[*offset] = 1;
		op = disasm->code->buf[(*offset)++];
		if (op == 0xff) {
			xfree(offnode);

			if (root == NULL) 
				return bt_code_new_leaf(OP_RETURN, 0, NULL);

			curnode->true = bt_code_new_leaf(OP_RETURN, 0, NULL);
			curnode->true->parent = curnode;

			break;
		}

		breakAfterFunc = (op & 0x80) ? 1 : 0;
		op &= 0x7f;

		if (op > 0x46) {
			abort();
			printf("Error: Bad opcode at %x (%x or %x)\n", *offset + disasm->base_offset - 1, op, op | 0x80);
			exit(1);
		}

		offnode->node = do_disasm_opcode(op, offset);

		/* Add the offset:cnode object to the offset hash table */
		bt_hash_add(offset_hash, offnode);

		if (root == NULL) {
			root = offnode->node;
			curnode = root;
		} else {
			curnode->true = offnode->node;
			offnode->node->parent = curnode;
		}

		/* OP_JUMP is the end of a code block too */
		if (offnode->node->opcode == OP_JUMP) 
			break;

		while (curnode->true != NULL) curnode = curnode->true;

		if (opFuncs[op].ifBlock) {
			if (!breakAfterFunc) {
				curnode->false = bt_code_new_leaf(OP_RETURN, 0, NULL);
				curnode->false->parent = curnode;

				curnode->true = do_disasm(offset);
				curnode->true->parent = curnode;

				break;
			} else {
				uint16_t true_offset;
				uint16_t save;

				disasm->bad_offset->buf[*offset] = 1;
				disasm->bad_offset->buf[*offset + 1] = 1;
				true_offset = str_read16le(&disasm->code->buf[*offset]) - disasm->base_offset;
				(*offset) += sizeof(uint16_t);

				/* 
				 * The false branch is always right after the true offset.
				 * There should be nothing between the end of the false branch
				 * and the beginning of the true branch.
				 */
				curnode->false = do_disasm(offset);
				curnode->false->parent = curnode;

				save = *offset;
				*offset = true_offset;

				curnode->true = do_disasm(offset);
				curnode->true->parent = curnode;

				if (save > *offset)
					*offset = save;

				break;
			}
		}
		
		if ((!breakAfterFunc) && (!opFuncs[op].clearBreak)) {
			curnode->true = bt_code_new_leaf(OP_RETURN, 0, NULL);
			curnode->true->parent = curnode;
			break;
		}
	}

	return root;
}

static b3data_t *searchForSquare(uint16_t offset)
{
	uint32_t i;

	for (i = 0; i < bt_array_length(disasm->curLevP->codeOffsets); i++) {
		b3data_t *tmp;

		tmp = bt_array_get(disasm->curLevP->codeOffsets, i);
		if (tmp->offset == offset)
			return tmp;
	}

	return NULL;
}

static uint8_t do__mfunc_extractCh(uint16_t *offset, uint8_t count)
{
	uint8_t rval = 0;

	while (count--) {
		_str_bitsLeft--;
		if (_str_bitsLeft < 0) {
			_str_curByte = disasm->code->buf[(*offset)++];
			_str_bitsLeft = 7;
		}

		rval <<= 1;
		if (_str_curByte & 0x80)
			rval |= 1;
		_str_curByte <<= 1;
	}

	return rval;
}

static uint8_t do__mfunc_unpackChar(uint16_t *offset)
{
	uint8_t ch;

do_loc_158a5:
	ch = do__mfunc_extractCh(offset, 5);
	if (ch) {
		if (ch == 30) {
			_str_capFlag = 1;
			goto do_loc_158a5;
		} else if (ch == 31) {
			ch = _str_Hialphabet[do__mfunc_extractCh(offset, 6)];
		} else {
			ch = _str_Loalphabet[ch];
		}

		if (_str_capFlag) {
			if (alphaFlags[ch] & 20)
				ch -= 0x20;
			_str_capFlag = 0;
		}
	}

	return ch;
}

/*
 * Convert a bit packed string to a btstring
 */
static btstring_t *map_getPackedString(void)
{
	btstring_t *rval;
	uint16_t stri;		/* String index */

	_str_bitsLeft = 0;
	_str_capFlag = 0;
	stri = 0;

	rval = bts_new(1024);

	while (rval->buf[stri++] = _mfunc_unpackChar());

	rval = bts_resize(rval, stri);

	return rval;
}

static btstring_t *do_map_getPackedString(uint16_t *offset)
{
	btstring_t *rval;
	uint16_t stri;

	_str_bitsLeft = 0;
	_str_capFlag = 0;
	stri = 0;

	rval = bts_new(1024);

	while (rval->buf[stri++] = do__mfunc_unpackChar(offset));

	return bts_resize(rval, stri);
}

static btstring_t *do_map_getMaskedString(uint16_t *offset)
{
	btstring_t *rval;
	uint8_t count = 0;

	rval = bts_new(1024);

	while (disasm->code->buf[*offset] != 0xff)
		rval->buf[count++] = disasm->code->buf[(*offset)++] & 0x7f;

	rval->buf[count++] = '\0';
	(*offset)++;

	return bts_resize(rval, count);
}

static uint32_t do_map_getString(uint16_t *offset, uint8_t type)
{
	btstring_t *str;
	uint32_t i;
	uint32_t label;
	uint16_t start;

	start = *offset;

	switch (type) {
	case STRING_PACKED:
		str = do_map_getPackedString(offset);
		break;
	case STRING_MASKED:
		str = do_map_getMaskedString(offset);
		break;
	}

	label = bt_string_hash_add(decomp->strtable, str);

	for (i = start; i < *offset; i++)
		disasm->bad_offset->buf[i] = 1;

	return label;
}

static bt_cnode_t *check_jump(uint16_t offset)
{
	bt_cnode_t *root;
	bt_cnode_t *rval;

	rval = b3_offset_search(offset);
	if (rval == NULL)
		return NULL;

	root = bt_code_new_leaf(OP_JUMP, 0, bt_opargs_new("w", offset));
	root->true = rval;

	return root;
}

static void skip_finished_offsets(uint16_t *offset)
{
	while (disasm->bad_offset->buf[*offset])
		(*offset)++;
}

static bt_cnode_t *do_disasm_opcode(uint8_t opcode, uint16_t *offset)
{
	bt_cnode_t *rval;
	bt_array_t *args;

	args = read_args(opcode, offset);
	rval = do_convertOpcode(offset, opcode, args);
	bt_array_free(args);

	return rval;
}

static bt_array_t *read_args(uint8_t opcode, uint16_t *offset)
{
	uint8_t arglen;
	uint8_t curarg = 0;
	uint8_t i, j;
	bt_array_t *args;

	arglen = strlen(opFuncs[opcode].args);

	if (arglen) {
		if (opFuncs[opcode].args[0] == 'B') 
			args = bt_array_new(1 + (disasm->code->buf[*offset] * 2), btarg_free);
		else
			args = bt_array_new(arglen, btarg_free);

		for (i = 0; i < arglen; i++) {
			switch (opFuncs[opcode].args[i]) {
			case 'b':
				disasm->bad_offset->buf[*offset] = 1;
				bt_array_set(args, curarg, btarg_new_byte(disasm->code->buf[(*offset)++]));
				break;
			case 'w':
				disasm->bad_offset->buf[*offset] = 1;
				disasm->bad_offset->buf[*offset + 1] = 1;
				bt_array_set(args, curarg, btarg_new_word(str_read16le(&disasm->code->buf[*offset])));
				*offset += 2;
				break;
			case 'o':
				disasm->bad_offset->buf[*offset] = 1;
				disasm->bad_offset->buf[*offset + 1] = 1;
				bt_array_set(args, curarg, btarg_new_offset(str_read16le(&disasm->code->buf[*offset])));
				*offset += 2;
				break;
			case 'B':
			{
				uint8_t ngroups;

				disasm->bad_offset->buf[*offset] = 1;
				ngroups = disasm->code->buf[(*offset)++];
				bt_array_set(args, curarg, btarg_new_byte(ngroups));
				curarg++;

				for (j = 0; j < ngroups; j++) {
					disasm->bad_offset->buf[*offset] = 1;
					bt_array_set(args, curarg, btarg_new_byte(disasm->code->buf[(*offset)++]));
					curarg++;

					disasm->bad_offset->buf[*offset] = 1;
					bt_array_set(args, curarg, btarg_new_byte(disasm->code->buf[(*offset)++]));
					curarg++;
				}
				curarg--;
				break;
			}
			case 's':
				bt_array_set(args, curarg, btarg_new_string(do_map_getString(offset, STRING_PACKED)));
				break;
			case 'S':
				bt_array_set(args, curarg, btarg_new_string(do_map_getString(offset, STRING_MASKED)));
				break;
			case 'M':
			{
				uint16_t saved_offset;
				uint16_t strno;

				disasm->bad_offset->buf[*offset] = 1;
				disasm->bad_offset->buf[*offset + 1] = 1;

				saved_offset = *offset + sizeof(uint16_t);

				*offset = str_read16le(&disasm->code->buf[*offset]) - disasm->base_offset;

#if 0
				if (b3_string_search(*offset) == NULL)
#endif
					strno = do_map_getString(offset, STRING_PACKED);

				bt_array_set(args, curarg, btarg_new_string(strno));

				*offset = saved_offset;

				break;
			}
			case 'z':
				if (disasm->code->buf[*offset] == 9)
					opFuncs[opcode].ifBlock = 1;
				else
					opFuncs[opcode].ifBlock = 0;

				disasm->bad_offset->buf[*offset] = 1;
				bt_array_set(args, curarg, btarg_new_byte(disasm->code->buf[(*offset)++]));
				break;
			default:
				printf("Didn't do it: arg %d: '0x%x'\n", i, opFuncs[opcode].args[i]);
				exit(1);
			}

			curarg++;
		}

		return args;
	}

	return NULL;
}


/********************************/
/*				*/
/*				*/
/*				*/
/********************************/

static void add_unresolved_offset(uint16_t offset, bt_cnode_t *node)
{
	b3_offset_t *new;

	new = (b3_offset_t *)xmalloc(sizeof(b3_offset_t));
	new->offset = offset;
	new->node = node;

	bt_stack_push(unresolved_offsets, new);
}

static void resolve_references(void)
{
	b3_offset_t *off;
	bt_cnode_t *search;
	bt_cnode_t *src;

	while ((off = bt_stack_pop(unresolved_offsets)) != NULL) {
		src = off->node;
		search = b3_offset_search(off->offset);

		/* 
		 * The jump location is outside of the current function.
		 * Replace the src jump with a call to do_disasm
		 */
		if (search == NULL) {
			uint16_t offset;

			assert(src->true == NULL);

			offset = off->offset - disasm->base_offset;
			debug("Offset 0x%x is outside of function\n", off->offset);
/*
			search = do_disasm(&offset);
			search->parent = src->parent;

			if (search->parent->true == src)
				search->parent->true = search;
			else
				search->parent->false = search;

			bt_array_free(src->args);
			xfree(src);
*/
		} else {
			src->true = search;
			src->true->parent = src;
		}

		xfree(off);
	}

}

/********************************/
/*				*/
/* Offset hash functions	*/
/*				*/
/********************************/

static void b3_offset_tree_free(void *bod)
{
	b3_offset_t *bo = bod;

	bt_cnode_free(bo->node);
	xfree(bo);
}

static void b3_offset_free(void *bod)
{
	xfree(bod);
}

static uint32_t b3_offset_get_hash_key(void *vbo)
{
	b3_offset_t *bo = vbo;

	return (bo->offset % OFFSETHASH_KEY);
}

static uint8_t b3_offset_compare(void *vsrc, void *vcmp)
{
	b3_offset_t *src = vsrc;
	b3_offset_t *cmp = vcmp;

	if (src->offset == cmp->offset)
		return 1;

	return 0;
}

static void b3_offset_dump(void *vbo)
{
	b3_offset_t *bo = vbo;

	printf("Offset: %x, label %d, refcount: %d\n", bo->offset);
}

static bt_cnode_t *b3_offset_search(uint16_t offset)
{
	b3_offset_t *srch;
	b3_offset_t *bo;

	srch = (b3_offset_t *)xmalloc(sizeof(b3_offset_t));
	srch->offset = offset;

	bo = bt_hash_search(offset_hash, srch);

	b3_offset_free(srch);

	return bo->node;
}

/********************************/
/*				*/
/* Conversion functions		*/
/*				*/
/********************************/

static bt_cnode_t *cnvB3stairs(uint16_t *offset, uint8_t dirFlag)
{
	bt_cnode_t *root;
	bt_cnode_t *tmp;
	b3data_t *sqData;
	uint8_t sqN;
	uint8_t sqE;
	uint8_t stairDir;

	/* printString(STAIRS_UP|STAIRS_DOWN) */
	if (dirFlag) {
		if (disasm->curDunP->direction)
			stairDir = STAIRS_UP;
		else
			stairDir = STAIRS_DOWN;
	} else {
		if (disasm->curDunP->direction)
			stairDir = STAIRS_DOWN;
		else
			stairDir = STAIRS_UP;
	}

#if 0
	if (stairDir) {
		root = bt_code_new_leaf(OP_PRINTSTRING,
			0,
			bt_opargs_new("B", bts_strcpy("STRING_STAIRS_UP"))
			);
	} else {
		root = bt_code_new_leaf(OP_PRINTSTRING,
			0,
			bt_opargs_new("B", bts_strcpy("STRING_STAIRS_DOWN"))
			);
	}
	tmp = root;

	/* getYesNo() */
	tmp->true = bt_code_new_branch(OP_GETYESNO, 0, NULL);
	tmp = tmp->true;

	/* Return if false */
	tmp->false = bt_code_new_leaf(OP_RETURN, 0, NULL);

	/* setOnSameSquare */
	tmp->true = bt_code_new_leaf(OP_SAMESQ, 0, NULL);
	tmp = tmp->true;
#endif

	/* teleport() */
	sqData = searchForSquare(*offset);
	if (dirFlag) {
		sqN = sqData->sqN + disasm->curLevP->deltaSqN - disasm->curDunP->levs[disasm->levno + 1]->deltaSqN;
		sqE = sqData->sqE + disasm->curLevP->deltaSqE - disasm->curDunP->levs[disasm->levno + 1]->deltaSqE;
	} else {
		if (disasm->levno) {
			sqN = sqData->sqN + disasm->curLevP->deltaSqN - disasm->curDunP->levs[disasm->levno - 1]->deltaSqN;
			sqE = sqData->sqE + disasm->curLevP->deltaSqE - disasm->curDunP->levs[disasm->levno - 1]->deltaSqE;
		} else {
			sqN = disasm->curLevP->exitSqN;
			sqE = disasm->curLevP->exitSqE;
		}
	}

	return bt_code_new_leaf(
		OP_STAIRS,
		0,
		bt_opargs_new("BbbB", (stairDir) ? bts_strcpy("STR_STAIRS_UP") : bts_strcpy("STR_STAIRS_DOWN"), sqN, sqE,
			getChangeLevelPath(disasm->dunno, disasm->levno, dirFlag))
		);
#if 0
	tmp->true = bt_code_new_leaf(OP_TELEPORT, 0,	\
		bt_opargs_new("bbB", sqN, sqE, getChangeLevelPath(disasm->dunno, disasm->levno, dirFlag))
		);
	tmp = tmp->true;

	/* Stairs probably always has a return next anyway */
	/* return_label */
	tmp->true = bt_code_new_leaf(OP_RETURN, 0, NULL);

	return root;
#endif
}

static bt_cnode_t *cnvB3teleport(bt_array_t *args)
{
	bt_cnode_t *new;

	uint8_t sqN;
	uint8_t sqE;
	uint8_t levno;

	sqN = btarg_get_byte(args, 0);
	sqE = btarg_get_byte(args, 1);
	levno = btarg_get_byte(args, 2);

	if (levno & 0x80) 
		levno = (levno & 0x7f) + 10;

	new = bt_code_new_leaf(OP_SAMESQ, 0, NULL);
	new->true = bt_code_new_leaf(OP_TELEPORT, 0, 
		bt_opargs_new("bbB", sqN, sqE, getLevelPath(levno2dun[levno], levno2lev[levno]))
		);

	return new;
}

static bt_cnode_t *cnvB3misc(bt_array_t *args)
{
	switch (btarg_get_byte(args, 0)) {
		case 3:
			return bt_code_new_leaf(
				OP_CNVTOGEOMANCER,
				0,
				NULL);
		case 4:
			return bt_code_new_leaf(
				OP_SCRYSITE,
				0,
				NULL);
		case 5:
			return bt_code_new_leaf(
				OP_DOVICTORY,
				0,
				NULL);
		case 9:
			return bt_code_new_leaf(
				OP_CHRONCHECK,
				0,
				NULL);
	}
				
}

static bt_cnode_t *cnvB3battle(bt_array_t *args, uint8_t cryFlag)
{
	bt_cnode_t *root = NULL;
	bt_cnode_t *cur;
	btstring_t *argstr;
	btarg_t *a;
	uint8_t i;
	uint32_t alen;

	if (cryFlag) {
		root = bt_code_new_leaf(
			OP_BATTLECRY,
			0,
			NULL
			);
	}

	alen = bt_array_length(args);
	argstr = bts_new(alen + 1);
	argstr->buf[alen] = '\0';

	argstr->buf[0] = 'b';

	for (i = 1; i < alen; i += 2) {
		a = bt_array_get(args, i);
		a->type = ARG_BTSTRING;
		a->bts = bts_copy(bt_array_get(disasm->curLevP->monsters, btarg_get_byte(args,i)));
		argstr->buf[i] = 'B';
		argstr->buf[i+1] = 'b';
	}

	cur = bt_code_new_branch(
		OP_BATTLE,
		0,
		bt_opargs_newv(argstr->buf, args)
		);

	if (root != NULL)
		root->true = cur;
	else
		root = cur;

	bts_free(argstr);

	return root;
}

static bt_cnode_t *cnvB3printString(bt_array_t *args, uint8_t clearFlag)
{
	bt_cnode_t *root = NULL;
	bt_cnode_t *cur;

	if (clearFlag) {
		root = bt_code_new_leaf(
			OP_CLEARWINDOW,
			0,
			NULL
			);
	}

	cur = bt_code_new_leaf(
		OP_PRINTSTRING,
		0,
		bt_opargs_newv("s", args)
		);

	if (root != NULL)
		root->true = cur;
	else
		root = cur;

	return root;
}

static bt_cnode_t *cnvB3action(bt_array_t *args)
{
	uint8_t action;
	bt_cnode_t *root = NULL;

	action = btarg_get_byte(args, 0);

	switch (action) {
	case 128:
		root = bt_code_new_branch(
			OP_ONACTION,
			0,
			bt_opargs_new("BB", bts_strcpy("ACT_USE"), bts_strcpy("USE_ACORN"))
			);
		break;
	case 129:
		root = bt_code_new_branch(
			OP_ONACTION,
			0,
			bt_opargs_new("BB", bts_strcpy("ACT_USE"), bts_strcpy("USE_DRINK"))
			);
		break;
	default:
		if (action > 128) {
			root = bt_code_new_branch(
				OP_ONSPELL,
				0,
				bt_opargs_new("b", action)
				);
		} else {
		root = bt_code_new_branch(
			OP_ONSPELL,
			0,	
			bt_opargs_new("B", getSpellAbbr(btarg_get_byte(args, 0)))
			);
		}
		break;
	}

	return root;
}
	
static bt_cnode_t *do_convertOpcode(uint16_t *offset, uint8_t opcode, bt_array_t *args)
{
	switch(opcode) {
		case 0:
			return cnvB3stairs(offset, STAIRS_DOWN);
		case 1:
			return cnvB3stairs(offset, STAIRS_UP);
		case 2:
			return cnvB3misc(args);
		case 3:
			return cnvB3teleport(args);
		case 4:
			return cnvB3battle(args, 1);
		case 5:		/* mfunc_clearPrintString	*/
		case 0x3a:	/* mfunc_clrPrintStringAtOffset */
			return cnvB3printString(args, 1);
		case 0x12:	/* mfunc_printString */
		case 0x45:	/* mfunc_printStringAtOffset */
			return cnvB3printString(args, 0);
		case 6:		/* mfunc_ffcode */
			return bt_code_new_leaf(
				OP_DUMMY,
				0,
				NULL);
			/* TODO */
		case 7:		/* mfunc_drawBigpic */
		{
			uint8_t picno;
			btstring_t *str;

			picno = btarg_get_byte(args, 0);
			if (picno == 0xfe) {
				return bt_code_new_leaf(
					OP_GRAYBIGPIC,
					0,
					NULL);
			} else {
				str = bts_strcpy(getPicMacro(btarg_get_byte(args,0)));

				return bt_code_new_leaf(
					OP_DRAWBIGPIC,
					0,
					bt_opargs_new("B", str)
				);
			}

			break;
		}
		case 8:		/* mfunc_setTitle*/
			return bt_code_new_leaf(
				OP_SETTITLE,
				0,
				bt_opargs_newv("s", args)
				);
		case 9:		/* mfunc_getCh	*/
			return bt_code_new_leaf(
				OP_GETCH,
				0,	\
				NULL);
			break;
		case 0xa:	/* mfunc_clearTextWindow() */
			return bt_code_new_leaf(
				OP_CLEARWINDOW, 
				0,	\
				NULL);
		case 0xb:	/* mfunc_isFlagSet()	*/
			return bt_code_new_branch(
				OP_ISFLAGSET, 
				0,	\
				bt_opargs_newv("b", args));
		case 0xc:	/* mfunc_isFlagNotSet()	*/
		{
			bt_cnode_t *root;

			root = bt_code_new_branch(
				OP_ISFLAGSET, 
				0,
				bt_opargs_newv("b", args));

			/* 
			 * Change the next instruction to jump if false 
			 */
			root->swap_test = 1;

			return root;
		}
		case 0xd:	/* mfunc_makeDoor */
			return bt_code_new_leaf(
				OP_DUMMY,
				0,
				NULL);
			/* TODO */
		case 0xe:	/* mfunc_setFlag */
			return bt_code_new_leaf(
				OP_SETFLAG,
				0,	\
				bt_opargs_new("bb", btarg_get_byte(args, 0), 1));
		case 0xf:	/* mfunc_clearFlag */
			return bt_code_new_leaf(
				OP_SETFLAG,
				0,	\
				bt_opargs_new("b", 0));
		case 0x10:	/* mfunc_ifCurSpellEQ */
			/* 
			 * TODO - Will have to look at this closer. The code here falls more under
			 * a onSpell() or onUse() paradigm.
			 */
			return cnvB3action(args);
#if 0
			if (btarg_get_byte(args, 0) < 128) {
			return bt_code_new_branch(
				OP_SPELLEQ,
				0,	
				bt_opargs_new("B", getSpellAbbr(btarg_get_byte(args, 0)))
				);
			}else {
			return bt_code_new_branch(
				OP_SPELLEQ,
				0,	
				bt_opargs_new("b", btarg_get_byte(args, 0))
				);
			}
#endif
#if 0
			return bt_code_new_leaf(
				0,	
				OP_SPELLEQ,
				bt_opargs_new("B", getSpellAbbr(btarg_get_byte(args, 0)))
				);
#endif
		case 0x11:	/* mfunc_setMapRval */
			/* 
			 * The mapRval function is used by the spell_cast function to determine
			 * whether on not the spell code was actually executed. This may or may
			 * not be used depending on how the game is implemented.
			 */
			return bt_code_new_leaf(
				OP_SETMAPRVAL,
				0,
				bt_opargs_new("b", 1)
				);
		case 0x13:	/* mfunc_doNothing */
			return bt_code_new_leaf(
				OP_DUMMY,
				0,
				NULL);
		case 0x14:	/* ifContainerLiquid */
			return bt_code_new_branch(
				OP_IFLIQUID,
				0,
				bt_opargs_new("B", getLiquidMacro(btarg_get_byte(args, 0)))
				);
		case 0x15:	/* getItem */
			return bt_code_new_branch(
				OP_GETITEM,
				0,
				bt_opargs_newv("bbb", args)
				);
		case 0x16:
			return bt_code_new_branch(
				OP_IFHASITEM,
				0,
				bt_opargs_new("B", getItemMacro(btarg_get_byte(args, 0)))
				);
		case 0x17:
		{
			bt_cnode_t *root;

			root =  bt_code_new_branch(
				OP_IFHASITEM,
				0,
				bt_opargs_new("B", getItemMacro(btarg_get_byte(args, 0)))
				);

			root->swap_test = 1;

			return root;
		}
		case 0x18:
			return bt_code_new_branch(
				OP_IFONNEWSQUARE,
				0,
				NULL);
		case 0x19:
			return bt_code_new_branch(
				OP_GETYESNO,
				0, \
				NULL);
		case 0x1a:	/* mfunc_goto */
		{
			bt_cnode_t *root;
			bt_cnode_t *next;

			root = bt_code_new_leaf(OP_JUMP, 0, bt_opargs_newv("w", args));

			next = b3_offset_search(btarg_get_word(args, 0));
			if (next == NULL) 
				add_unresolved_offset(btarg_get_word(args, 0), root);
			else
				root->true = b3_offset_search(btarg_get_word(args, 0));

			return root;
		}
		case 0x1b:
			return cnvB3battle(args, 0);
		case 0x1c:
			return bt_code_new_leaf(
				OP_SAMESQ,
				0, \
				NULL);
		case 0x1d:
			return bt_code_new_leaf(
				OP_TURNAROUND,
				0, \
				NULL);
		case 0x1e:
			return bt_code_new_leaf(
				OP_REMOVEITEM,
				0, \
				bt_opargs_new("B", getItemMacro(btarg_get_byte(args, 0)))
				);
		case 0x1f:
			return bt_code_new_leaf(
				OP_INCREGISTER, \
				0, \
				bt_opargs_new("b", btarg_get_byte(args, 0))
				);
		case 0x20:
			return bt_code_new_leaf(
				OP_DECREGISTER, \
				0, \
				bt_opargs_new("b", btarg_get_byte(args, 0))
				);
		case 0x21:
			return bt_code_new_branch(
				OP_CMPREGISTER,
				0, \
				bt_opargs_new("bw", btarg_get_byte(args, 0), 0)
				);
		case 0x22:
		{
			bt_cnode_t *root;

			root = bt_code_new_branch(
				OP_CMPREGISTER,
				0, \
				bt_opargs_new("bw", btarg_get_byte(args, 0), 0)
				);
			root->swap_test = 1;

			return root;
		}
		case 0x23:
			return bt_code_new_leaf(
				OP_DRAINHP, \
				0, \
				bt_opargs_new("w", btarg_get_word(args, 0))
				);
		case 0x24:
			return bt_code_new_branch(
				OP_IFINBOX, \
				0, \
				bt_opargs_newv("bbbb", args)
				);
		case 0x25:
			return bt_code_new_leaf(
				OP_SETLIQUID,
				0, \
				bt_opargs_new("B", getLiquidMacro(btarg_get_byte(args, 0)))
				);
		case 0x26:
			return bt_code_new_leaf(
				OP_ADDTOINV,
				0,
				bt_opargs_newv("b", args)
				);
		case 0x27:
			return bt_code_new_leaf(
				OP_SUBFROMINV,
				0,
				bt_opargs_newv("b", args)
				);
		case 0x28:
			return bt_code_new_leaf(
				OP_ADDTOREG,
				0,
				bt_opargs_newv("bw", args)
				);
		case 0x29:
			return bt_code_new_leaf(
				OP_SUBFROMREG,
				0,
				bt_opargs_newv("bw", args)
				);
		case 0x2a:
			return bt_code_new_leaf(
				OP_SETDIRFACING,
				0,
				bt_opargs_new("B", getDirectionMacro(btarg_get_byte(args, 0)))
				);
		case 0x2b:	
			return bt_code_new_leaf(
				OP_READSTRING,
				0,
				NULL);
		case 0x2c:	
			return bt_code_new_branch(
				OP_STRCMP,
				0,
				bt_opargs_newv("s", args)
				);
		case 0x2d:
			return bt_code_new_leaf(
				OP_READNUMBER,
				0,
				bt_opargs_newv("b", args)
				);
		case 0x2e:
			return bt_code_new_branch(
				OP_GETCHARACTER,
				0,
				NULL);
		case 0x2f:
			return bt_code_new_branch(
				OP_IFGIVEGOLD,
				0,
				bt_opargs_newv("b", args)
				);
		case 0x30:
			return bt_code_new_leaf(
				OP_RECEIVEGOLD,
				0,
				bt_opargs_newv("b", args)
				);
		case 0x31:
		case 0x32:
		case 0x33:
			/* TODO - Get the jge,jlt,jeq */
			return bt_code_new_branch(
				OP_CMPREGISTER,
				0,
				bt_opargs_newv("bw", args)
				);
		case 0x34:
			return bt_code_new_leaf(
				OP_LEARNSPELL,
				0,
				bt_opargs_new("B", getSpellAbbr(btarg_get_byte(args, 0)))
				);
		case 0x35:
			return bt_code_new_leaf(
				OP_SETREGISTER,
				0,
				bt_opargs_newv("bw", args)
				);
		case 0x36:
			return bt_code_new_branch(
				OP_CHHASITEM,
				0,
				bt_opargs_new("B", getItemMacro(btarg_get_byte(args, 0)))
				);
		case 0x37:
			return bt_code_new_leaf(
				OP_PACKINV,
				0,
				NULL);
		case 0x38:
		{
			btstring_t *mon;

			mon = bts_copy(bt_array_get(disasm->curLevP->monsters, btarg_get_byte(args,0)));

			return bt_code_new_branch(
				OP_ADDMON,
				0,
				bt_opargs_new("B", mon)
				);
		}
		case 0x39:
			return bt_code_new_branch(
				OP_ISMONINPARTY,
				0,
				bt_opargs_newv("s", args)
			);
		case 0x3b:
			return bt_code_new_branch(
				OP_ISNIGHT,
				0,
				NULL);
		case 0x3c:
			return bt_code_new_leaf(
				OP_RMMON,
				0,
				NULL);
		case 0x3f:
			return bt_code_new_leaf(
				OP_SETQUESTFLAG, 
				0,
				bt_opargs_newv("b", args)
				);
		case 0x42:
			return bt_code_new_branch(
				OP_ISWILDSQSET,
				0,
				bt_opargs_newv("b", args)
				);
		case 0x43:
			return bt_code_new_leaf(
				OP_SETWILDSQ,
				0,
				bt_opargs_newv("b", args)
				);
		case 0x44:
			return bt_code_new_branch(
				OP_ISCLASS,
				0,
				bt_opargs_newv("b", args)
				);
		case 0x46:
		{
			bt_cnode_t *root;

			root = bt_code_new_leaf(
				OP_GRAYBIGPIC,
				0, \
				NULL);
			root->true = cnvB3teleport(args);

			return root;
		}
		default:
			return bt_code_new_leaf(
				opcode,
				0,
				NULL
				);
	}
}
			

static void convertB3code(void)
{
	uint32_t i;
	bt_cnode_t *tmp;
	b3data_t *sq;

	tree_disasm_level();
}

/********************************/
/*				*/
/* Public Functions		*/
/*				*/
/********************************/

void disasm_code(dun_t *d, uint8_t dunno)
{
	uint8_t i;

	disasm = (b3_disasm_t *)xmalloc(sizeof(b3_disasm_t));

	disasm->dunno = dunno;
	disasm->curDunP = d;

	for (i = 0; i < disasm->curDunP->nlevs; i++) {
		disasm->levno = i;
		disasm->curLevP = disasm->curDunP->levs[i];
		convertB3code();

		/*outputCodeXML(dungeon->levs[currentLevel]->code);*/
		printf("===================================================\n");
	}

	xfree(disasm);
}

