#ifndef _LIBCNV_CNV_CODE_H
#define _LIBCNV_CNV_CODE_H

#include <stdint.h>
#include <bt_string.h>
#include <bt_gds.h>
#include <stdarg.h>

/********************************/
/*				*/
/* Macros			*/
/*				*/
/********************************/

#define ARG_BYTE	1
#define ARG_WORD	2
#define ARG_STRING	3
#define ARG_OFFSET	4
#define ARG_BTSTRING	5

#define STAIRS_DOWN	0
#define STAIRS_UP	1

#define NODE_LEAF	1
#define NODE_BRANCH	2

/********************************/
/*				*/
/* Opcodes			*/
/*				*/
/********************************/

#define OP_NONE		0
#define OP_PRINTSTRING	1
#define OP_TELEPORT	2
#define OP_BATTLE	3
#define OP_CLEARWINDOW	4
#define OP_GETYESNO	5
#define OP_SAMESQ	6
#define OP_DRAWBIGPIC	7
#define OP_SETTITLE	8
#define OP_GETCH	9
#define OP_ISFLAGSET	10
#define OP_SETFLAG	11
#define OP_SPELLEQ	12
#define OP_IFLIQUID	13
#define OP_IFHASITEM	14
#define OP_BATTLECRY	15
#define OP_GRAYBIGPIC	16
#define OP_JMPFALSE	17
#define OP_JMPTRUE	18
#define OP_RETURN	19
#define OP_SETMAPRVAL	20
#define OP_GETITEM	21
#define OP_IFONNEWSQUARE	22
#define OP_JUMP		23
#define OP_TURNAROUND	24
#define OP_REMOVEITEM	25
#define OP_INCREGISTER	26
#define OP_DECREGISTER	27
#define OP_CMPREGISTER	28
#define OP_DRAINHP	29
#define OP_IFINBOX	30
#define OP_SETLIQUID	31
#define OP_ADDTOINV	32
#define OP_SUBFROMINV	33
#define OP_ADDTOREG	34
#define OP_SUBFROMREG	35
#define OP_SETDIRFACING	36
#define OP_READSTRING	37
#define OP_STRCMP	38
#define OP_READNUMBER	39
#define OP_GETCHARACTER	40
#define OP_IFGIVEGOLD	41
#define OP_RECEIVEGOLD	42
#define OP_LEARNSPELL	43
#define OP_SETREGISTER	44
#define OP_CHHASITEM	45
#define OP_PACKINV	46
#define OP_ADDMON	47
#define OP_ISMONINPARTY	48
#define OP_ISNIGHT	49
#define OP_RMMON	50
#define OP_SETQUESTFLAG	51
#define OP_ISWILDSQSET	52
#define OP_SETWILDSQ	53
#define OP_ISCLASS	54
#define OP_CNVTOGEOMANCER	55
#define OP_SCRYSITE	56
#define OP_DOVICTORY	57
#define OP_CHRONCHECK	58
#define OP_STAIRS	59
#define OP_ONSPELL	60
#define OP_ONACTION	61
#define OP_DUMMY	62


typedef struct {
	uint16_t number;
	btstring_t *_string;
} bt_string_element;

typedef struct {
	uint8_t type;
	uint8_t _byte;
	uint16_t _word;
	btstring_t *bts;
} btarg_t;

typedef struct _bt_cnode_t {
	uint8_t type;
	uint8_t label;
	uint8_t opcode;
	uint8_t swap_test;
	bt_array_t *args;
	struct _bt_cnode_t *true;
	struct _bt_cnode_t *false;
	struct _bt_cnode_t *parent;
} bt_cnode_t;

typedef struct {
	uint16_t label;
	uint16_t offset;
	bt_cnode_t *root;
} bt_map_function_t;

/********************************/
/*				*/
/* Public interface		*/
/*				*/
/********************************/

#define btarg_get_type(op,index)	((btarg_t *)(bt_array_get((op), (index))))->type
#define btarg_get_byte(op,index)	((btarg_t *)(bt_array_get((op), (index))))->_byte
#define btarg_get_word(op,index)	((btarg_t *)(bt_array_get((op), (index))))->_word
#define btarg_get_string(op,index)	((btarg_t *)(bt_array_get((op), (index))))->_word
#define btarg_get_offset(op,index)	((btarg_t *)(bt_array_get((op), (index))))->_word
#define btarg_get_btstring(op,index)	((btarg_t *)(bt_array_get((op), (index))))->bts


btarg_t *btarg_new(void);
void btarg_free(void *);
bt_cnode_t *bt_code_new_leaf(uint8_t opcode, uint8_t label, bt_array_t *args);
bt_cnode_t *bt_code_new_branch(uint8_t opcode, uint8_t label, bt_array_t *args);

bt_array_t *bt_opargs_new(uint8_t *format, ...);
bt_array_t *bt_opargs_newv(uint8_t *format, bt_array_t *args);

void bt_cnode_free(bt_cnode_t *bc);

bt_map_function_t *bt_map_function_new(void);
void bt_map_function_free(bt_map_function_t *bmf);

btarg_t *btarg_new_byte(uint8_t val);
btarg_t *btarg_new_word(uint16_t val);
btarg_t *btarg_new_string(uint16_t val);
btarg_t *btarg_new_offset(uint16_t val);
btarg_t *btarg_new_btstring(btstring_t *val);


#endif
