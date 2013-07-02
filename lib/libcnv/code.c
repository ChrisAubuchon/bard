#include <cnvlib.h>
#include <cnv_code.h>
#include <cnv_int.h>
#include <string.h>

/********************************/
/*				*/
/* Static data			*/
/*				*/
/********************************/

static uint8_t *opcodes[] = {
	"none", "print_string", "teleport", "battle", "clearwindow",
	"getyesno", "samesq", "drawbigpic", "settitle", "getch",
	"isflagset", "setflag", "spelleq", "ifliquid", "ifhasitem",
	"battlecry", "graybigpic", "jmpfalse", "jmptrue", "return",
	"setmaprval", "getitem", "ifonnewsquare", "jump", "turnaround",
	"removeitem", "incregister", "decregister", "cmpregister", "drainhp",
	"ifinbox", "setliquid", "addtoinv", "subfrominv", "addtoreg",
	"subfromreg", "setdirfacing", "readstring", "strcmp", "readnumber",
	"getcharacter", "ifgivegold", "receivegold", "learnspell", "setregister",
	"chhasitem", "packinv", "addmon", "ismoninparty", "isnight", "rmmon",
	"setquestflag", "iswildsqset", "setwildsq", "isclass", "cnvtogeomancer",
	"scrysite", "victory", "chronocheck", "stairs", "onSpell", "onAction",
	"dummy"
};

/********************************/
/*				*/
/* Local function prototypes	*/
/*				*/
/********************************/

static void bt_string_element_free(bt_string_element *bse);
static bt_cnode_t *bt_cnode_new(void);
/*static void bt_cnode_free(bt_cnode_t *bc);*/
static bt_cnode_t *_bt_code_new_node(uint8_t type, uint8_t label, uint8_t opcode, bt_array_t *args);

/********************************/
/*				*/
/* Local Functions		*/
/*				*/
/********************************/

static bt_cnode_t *_bt_code_new_node(uint8_t type, uint8_t label, uint8_t opcode, bt_array_t *args)
{
	bt_cnode_t *new;

	new = bt_cnode_new();
	new->type = type;
	new->label = label;
	new->opcode = opcode;
	new->args = args;

	return new;
}

static void bt_string_element_free(bt_string_element *bse)
{
	bts_free(bse->_string);
	free(bse);
}

void bt_cnode_free(bt_cnode_t *bc)
{
	if (bc == NULL)
		return;

	if (bc->opcode != OP_JUMP) {

	if (bc->true != NULL)
		bt_cnode_free(bc->true);

	if (bc->false != NULL)
		bt_cnode_free(bc->false);
	}

	bt_array_free(bc->args);
	free(bc);
}

/********************************/
/*				*/
/* Public Interface		*/
/*				*/
/********************************/

bt_map_function_t *bt_map_function_new(void)
{
	bt_map_function_t *new;

	new = (bt_map_function_t *)xzalloc(sizeof(bt_map_function_t));

	return new;
}

void bt_map_function_free(bt_map_function_t *bmf)
{
	bt_cnode_free(bmf->root);
	free(bmf);
}

bt_cnode_t *bt_cnode_new(void)
{
	bt_cnode_t *new;

	new = (bt_cnode_t *)xzalloc(sizeof(bt_cnode_t));

	return new;
}

btarg_t *btarg_new(void)
{
	return (btarg_t *)xzalloc(sizeof(btarg_t));
}

btarg_t *btarg_new_byte(uint8_t val)
{
	btarg_t *rval;

	rval = (btarg_t *)xzalloc(sizeof(btarg_t));
	rval->type = ARG_BYTE;
	rval->_byte = val;

	return rval;
}
btarg_t *btarg_new_word(uint16_t val)
{
	btarg_t *rval;

	rval = (btarg_t *)xzalloc(sizeof(btarg_t));
	rval->type = ARG_WORD;
	rval->_word = val;

	return rval;
}

btarg_t *btarg_new_string(uint16_t val)
{
	btarg_t *rval;

	rval = (btarg_t *)xzalloc(sizeof(btarg_t));
	rval->type = ARG_STRING;
	rval->_word = val;

	return rval;
}

btarg_t *btarg_new_offset(uint16_t val)
{
	btarg_t *rval;

	rval = (btarg_t *)xzalloc(sizeof(btarg_t));
	rval->type = ARG_OFFSET;
	rval->_word = val;

	return rval;
}

btarg_t *btarg_new_btstring(btstring_t *val)
{
	btarg_t *rval;

	rval = (btarg_t *)xzalloc(sizeof(btarg_t));
	rval->type = ARG_BTSTRING;
	rval->bts = val;

	return rval;
}

void btarg_free(void *vbta)
{
	btarg_t *bta = vbta;

	if (bta->type == ARG_BTSTRING)
		bts_free(bta->bts);
	free(bta);
}

bt_array_t *bt_opargs_newv(uint8_t *format, bt_array_t *args)
{
	bt_array_t *rval;
	btarg_t *new;
	btarg_t *old;
	uint8_t arglen;
	uint8_t i;

	arglen = strlen(format);
	rval = bt_array_new(arglen, btarg_free);

	for (i = 0; i < arglen; i++) {
		new = btarg_new();

		old = bt_array_get(args, i);

		switch (format[i]) {
		case 'b':
			new->type = ARG_BYTE;
			new->_byte = old->_byte;
			break;
		case 'w':
			new->type = ARG_WORD;
			new->_word = old->_word;
			break;
		case 's':
			new->type = ARG_STRING;
			new->_word = old->_word;
			break;
		case 'o':
			new->type = ARG_OFFSET;
			new->_word = old->_word;
			break;
		case 'B':
			new->type = ARG_BTSTRING;
			new->bts = bts_copy(old->bts);
			break;
		}
		bt_array_set(rval, i, new);
	}

	return rval;
}

			


bt_array_t *bt_opargs_new(uint8_t *format, ...)
{
	bt_array_t *rval;
	btarg_t *new;
	uint8_t arglen;
	uint8_t i;
	va_list argv;

	va_start(argv, format);

	arglen = strlen(format);
	rval = bt_array_new(arglen, btarg_free);

	for (i = 0; i < arglen; i++) {
		new = btarg_new();

		switch (format[i]) {
		case 'b':
			new->type = ARG_BYTE;
			new->_byte = (uint8_t)va_arg(argv, int);
			break;
		case 'w':
			new->type = ARG_WORD;
			new->_word = (uint16_t)va_arg(argv,int);
			break;
		case 's':
			new->type = ARG_STRING;
			new->_word = (uint16_t)va_arg(argv,int);
			break;
		case 'o':
			new->type = ARG_OFFSET;
			new->_word = (uint16_t)va_arg(argv,int);
			break;
		case 'B':
			new->type = ARG_BTSTRING;
			new->bts = (btstring_t *)va_arg(argv, btstring_t *);
			break;
		}

		bt_array_set(rval, i, new);
	}

	va_end(format);

	return rval;
}

bt_cnode_t *bt_code_new_leaf(uint8_t opcode, uint8_t label, bt_array_t *args)
{
	return _bt_code_new_node(NODE_LEAF, label, opcode, args);
}

bt_cnode_t *bt_code_new_branch(uint8_t opcode, uint8_t label, bt_array_t *args)
{
	return _bt_code_new_node(NODE_BRANCH, label, opcode, args);
}

void outputCodeTreeXML(bt_cnode_t *root, uint8_t indent)
{
	bt_cnode_t *tmp = root;
	uint8_t i;
	btarg_t *arg;

	while (tmp != NULL) {
		iprintf(0, "(%x) %s(", tmp->opcode, opcodes[tmp->opcode]);
		for (i = 0; i < bt_array_length(tmp->args); i++) {
			if (i)
				printf(", ");

			arg = bt_array_get(tmp->args, i);
			switch (arg->type) {
			case ARG_BYTE:
				printf("%d", arg->_byte);
				break;
			case ARG_WORD:
				printf("0x%x", arg->_word);
				break;
			case ARG_STRING:
				printf("STRING_%d", arg->_word);
				break;
			case ARG_OFFSET:
				printf("LABEL_%d", arg->_word);
				break;
			case ARG_BTSTRING:	
				printf("%s", arg->bts->buf);
				break;
			}
		}
		printf(")\n");
		if (tmp->type == NODE_LEAF) {
			if (tmp->opcode == OP_JUMP)
				return;
			tmp = tmp->true;
		} else {
			iprintf(0, "{\n");
			outputCodeTreeXML(tmp->true, indent + 2);
			iprintf(0, "} else {\n");
			outputCodeTreeXML(tmp->false, indent + 2);
			iprintf(0, "}\n");
			return;
		}
	}
}

#if 0
void outputCodeXML(bt_code_t *btc)
{
	bt_code_element *elem;
	bt_llelem_t *node;
	uint8_t i;
	btarg_t *arg;

	for (node = btc->head; node != NULL; node = node->next) {
		elem = bt_llelem_cast(node, bt_code_element *);

		(elem->linenum) ? printf("LABEL_%d:\t\t", elem->linenum) : printf("\t\t\t");
		printf("%s(", opcodes[elem->opcode]);
		/*printf("Label: %d, Opcode = %d, args = (", elem->linenum, elem->opcode);*/
		for (i = 0; i < elem->args->size; i++) {
			if (i)
				printf(", ");

			arg = btlist_cast(elem->args->list[i], btarg_t *);
			switch (arg->type) {
			case ARG_BYTE:
				printf("%d", arg->_byte);
				break;
			case ARG_WORD:
				printf("%d", arg->_word);
				break;
			case ARG_STRING:
				printf("STRING_%d", arg->_word);
				break;
			case ARG_OFFSET:
				printf("LABEL_%d", arg->_word);
				break;
			case ARG_BTSTRING:	
				printf("%s", arg->bts->buf);
				break;
			}
		}
		printf(")\n");
	}
}

void outputLevelFunctionsXML(bt_llist_t *funcs)
{
	bt_llelem_t *elem;
	bt_map_function_t *bmf;

	elem = bt_llist_get_head(funcs);
	for (; elem != NULL; elem = elem->next) {
		bmf = bt_llelem_cast(elem, bt_map_function_t *);

		printf("function_%d {\n", bmf->label);
		outputCodeTreeXML(bmf->root, 0);
		printf("}\n");
	}
}
#endif
