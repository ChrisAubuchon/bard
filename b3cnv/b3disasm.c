#include <b3lib.h>
#include <dun.h>

#define DEBUG 1
#include <debug.h>

typedef void (opcodeFunc_t)(FILE *, btstring_t *, uint16_t *);

/********************************/
/*				*/
/* Global variables		*/
/*				*/
/********************************/

char *program_name = "b3disasm";

btstring_t	*inputDir	= NULL;
btstring_t	*outputDir	= NULL;

/********************************/
/*				*/
/* opcode prototypes		*/
/*				*/
/********************************/

static void	_op_getElse(FILE *, btstring_t *, uint16_t *);

static void	op_stairsDown(FILE *, btstring_t *, uint16_t *);
static void	op_stairsUp(FILE *, btstring_t *, uint16_t *);
static void	op_teleport(FILE *, btstring_t *, uint16_t *);
static void	op_clearPrint(FILE *, btstring_t *, uint16_t *);
static void	op_clearSpecial(FILE *, btstring_t *, uint16_t *);
static void	op_bigpic(FILE *, btstring_t *, uint16_t *);
static void	op_setTitle(FILE *, btstring_t *, uint16_t *);
static void	op_anyKey(FILE *, btstring_t *, uint16_t *);
static void	op_clear(FILE *, btstring_t *, uint16_t *);
static void	op_isGFlagSet(FILE *, btstring_t *, uint16_t *);
static void	op_isGFlagClear(FILE *, btstring_t *, uint16_t *);
static void	op_setGFlag(FILE *, btstring_t *, uint16_t *);
static void	op_ifSpellEq(FILE *, btstring_t *, uint16_t *);
static void	op_setMapRval(FILE *, btstring_t *, uint16_t *);
static void	op_print(FILE *, btstring_t *, uint16_t *);
static void	op_nothing(FILE *, btstring_t *, uint16_t *);
static void	op_getItem(FILE *, btstring_t *, uint16_t *);
static void	op_partyHasItem(FILE *, btstring_t *, uint16_t *);
static void	op_battleNoCry(FILE *, btstring_t *, uint16_t *);
static void	op_sameSquare(FILE *, btstring_t *, uint16_t *);
static void	op_getYesNo(FILE *, btstring_t *, uint16_t *);
static void	op_goto(FILE *, btstring_t *, uint16_t *);
static void	op_setSameSquare(FILE *, btstring_t *, uint16_t *);
static void	op_getInput(FILE *, btstring_t *, uint16_t *);
static void	op_ifLastInput(FILE *, btstring_t *, uint16_t *);
static void	op_userHasItem(FILE *, btstring_t *, uint16_t *);
static void	op_takeItem(FILE *, btstring_t *, uint16_t *);

/********************************/
/*				*/
/* Local variables		*/
/*				*/
/********************************/

/* Packed string bits left */
static int8_t	sp_bitsLeft;

/* Opcode breakAfter */
static uint8_t	op_breakAfter;

/* Level pointer */
static b3level_t	*currentLevel;

static opcodeFunc_t *opcodes[] = {
/*   0*/	op_stairsDown, op_stairsUp, NULL, op_teleport, 
/*   4*/	NULL, op_clearPrint, op_clearSpecial, op_bigpic, 
/*   8*/	op_setTitle, op_anyKey, op_clear, op_isGFlagSet, 
/*  12*/	op_isGFlagClear, NULL, op_setGFlag, NULL, 
/*  16*/	op_ifSpellEq, op_setMapRval, op_print, op_nothing, 
/*  20*/	NULL, op_getItem, op_partyHasItem, NULL, 
/*  24*/	op_sameSquare, op_getYesNo, op_goto, op_battleNoCry, 
/*  28*/	op_setSameSquare, NULL, NULL, NULL, 
/*  32*/	NULL, NULL, NULL, NULL, 
/*  36*/	NULL, NULL, NULL, NULL, 
/*  40*/	NULL, NULL, NULL, op_getInput, 
/*  44*/	op_ifLastInput, NULL, NULL, NULL, 
/*  48*/	NULL, NULL, NULL, NULL, 
/*  52*/	NULL, NULL, op_userHasItem, op_takeItem, 
/*  56*/	NULL, NULL, NULL, NULL, 
/*  60*/	NULL, NULL, NULL, NULL, 
/*  64*/	NULL, NULL, NULL, NULL, 
/*  68*/	NULL, NULL, NULL, NULL, 
/*  72*/	NULL, NULL, NULL, NULL, 
/*  76*/	NULL, NULL, NULL, NULL, 
/*  80*/	NULL, NULL, NULL, NULL, 
/*  84*/	NULL, NULL, NULL, NULL, 
/*  88*/	NULL, NULL, NULL, NULL, 
/*  92*/	NULL, NULL, NULL, NULL, 
/*  96*/	NULL, NULL, NULL, NULL, 
/* 100*/	NULL, NULL, NULL, NULL, 
/* 104*/	NULL, NULL, NULL, NULL, 
/* 108*/	NULL, NULL, NULL, NULL, 
/* 112*/	NULL, NULL, NULL, NULL,
/* 116*/	NULL, NULL, NULL, NULL,
/* 120*/	NULL, NULL, NULL, NULL,
/* 124*/	NULL, NULL, NULL, NULL
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

/********************************/
/*				*/
/* Local prototypes		*/
/*				*/
/********************************/

static void	usage(void);
static void	dump_disasm(uint8_t, uint8_t);
static void	disasmCode(FILE *, btstring_t *, uint16_t);
static void	disasmSpecial(FILE *, btstring_t *, uint16_t, uint16_t);
static uint32_t	disasmConditional(FILE *, btstring_t *, uint16_t *);
static void	disasmOpcode(FILE *, btstring_t *, uint16_t *);
static uint32_t	disasmIf(FILE *, btstring_t *, uint16_t *);

static void	printArgs(FILE *, btstring_t *, uint16_t *, const char *);

static void	printPackedString(FILE *, btstring_t *, uint16_t *);
static void	printMaskedString(FILE *, btstring_t *, uint16_t *);
static void	printBigpic(FILE *, uint8_t);
static void	printSpell(FILE *, uint8_t);
static void	printItem(FILE *, uint8_t);
static void	printMonster(FILE *, uint8_t);
static uint8_t	sp_unpackChar(btstring_t *, uint16_t *);
static uint8_t	sp_extractCh(btstring_t *, uint16_t *, uint8_t);


/********************************/
/*				*/
/* Local functions		*/
/*				*/
/********************************/

/*
 * usage()
 */
static void usage(void)
{
	fprintf(stderr, "b3disasm -d inputDir -o outputDir\n\n");
}

/********************************/
/*				*/
/* Print Functions		*/
/*				*/
/********************************/

/*
 * printArgs()
 */
static void printArgs(FILE *fp, btstring_t *code, uint16_t *offset, 
	const char *format)
{
	uint32_t	arglen;
	uint32_t	i;

	arglen = strlen(format);

	if (arglen) {
		for (i = 0; i < arglen; i++) {
			if (i)
				fprintf(fp, ", ");

			switch(format[i]) {
			case 'b':	/* byte */
				fprintf(fp, "%d", code->buf[(*offset)++]);
				break;
			case 'B':	/* Bigpic image */
				printBigpic(fp, code->buf[(*offset)++]);
				break;
			case 'C':	/* Spell */
				printSpell(fp, code->buf[(*offset)++]);
				break;
			case 'f':	/* Flag */
			{
				uint8_t		flag;
				uint8_t		mask;

				flag	= code->buf[*offset] >> 3;
				mask	= code->buf[*offset] & 7;
				fprintf(fp, "%d", (flag * 8) + mask);
				(*offset)++;
				break;
			}
			case 'I':	/* Item */
				printItem(fp, code->buf[(*offset)++]);
				break;
			case 'M':	/* Monster */
				printMonster(fp, code->buf[(*offset)++]);
				break;
			case 'S':	/* Masked String */
				fprintf(fp, "\"");
				fflush(fp);
				printMaskedString(fp, code, offset);
				fprintf(fp, "\"");
				break;
			case 's':	/* Packed String */
				fprintf(fp, "\"");
				printPackedString(fp, code, offset);
				fprintf(fp, "\"");
				break;
			case 'x':	/* Hex byte */
				fprintf(fp, "0x%02x", code->buf[(*offset)++]);
				break;
			}
		}
	}
}

/*
 * printPackedString()
 */
static void printPackedString(FILE *fp, btstring_t *code, uint16_t *offset)
{
	btstring_t	*rval;
	uint32_t	stri	= 0;

	sp_bitsLeft = 0;

	rval	= bts_new(1024);

	while (rval->buf[stri++] = sp_unpackChar(code, offset));

	bts_resize(rval, stri);

	fprintf(fp, "%.*s", rval->size, rval->buf);

	bts_free(rval);
}

/*
 * printMaskedString()
 */
static void printMaskedString(FILE *fp, btstring_t *code, uint16_t *offset)
{
	btstring_t	*rval;
	uint32_t	stri	= 0;

	rval = bts_new(1024);

	while (code->buf[*offset] != 0xff) 
		rval->buf[stri++] = code->buf[(*offset)++] & 0x7f;

	rval->buf[stri++] = '\0';
	(*offset)++;

	bts_resize(rval, stri);

	fprintf(fp, "%.*s", rval->size, rval->buf);

	bts_free(rval);
}

/*
 * printBigpic()
 */
static void printBigpic(FILE *fp, uint8_t pic)
{
	pic_t		p;
	btstring_t	*rval;

	indexToPic(pic, &p);
	rval = getPicMacro(&p);

	fprintf(fp, "%.*s", rval->size, rval->buf);

	bts_free(rval);
}

/*
 * printSpell()
 */
static void printSpell(FILE *fp, uint8_t spell)
{
	btstring_t	*rval;

	if (spell < 125) {
		rval = getSpellAbbr(spell);

		fprintf(fp, "%.*s", rval->size, rval->buf);

		bts_free(rval);
	} else {
		fprintf(fp, "%d", spell);
	}
}

/*
 * printItem()
 */
static void printItem(FILE *fp, uint8_t item)
{
	btstring_t	*rval;

	rval	= getItemMacro(item);

	fprintf(fp, "%.*s", rval->size, rval->buf);

	bts_free(rval);
}

/*
 * printMonster()
 */
static void printMonster(FILE *fp, uint8_t monster)
{
	btstring_t	*rval;

	rval	= getLevelMonster(currentLevel->monIndex, monster);

	fprintf(fp, "%.*s", rval->size, rval->buf);

	bts_free(rval);
}

/********************************/
/*				*/
/* String functions		*/
/*				*/
/********************************/

/*
 * sp_extractCh()
 */
static uint8_t sp_extractCh(btstring_t *code, uint16_t *offset, uint8_t count)
{
	uint8_t		rval = 0;

	static uint8_t	curByte;

	while (count--) {
		sp_bitsLeft--;
		if (sp_bitsLeft < 0) {
			curByte = code->buf[(*offset)++];
			sp_bitsLeft = 7;
		}


		rval <<= 1;
		if (curByte & 0x80)
			rval |= 1;
		curByte <<= 1;
	}


	return rval;
}

/*
 * unpackChar()
 */
static uint8_t sp_unpackChar(btstring_t *code, uint16_t *offset)
{
	uint8_t		ch;
	uint8_t		isCapital	= 0;

	while (1) {
		ch = sp_extractCh(code, offset, 5);
		if (!ch)
			break;

		if (ch == 30) {
			isCapital = 1;
		} else {
			if (ch == 31)
				ch = _str_Hialphabet[
					sp_extractCh(code, offset, 6)];
			else
				ch = _str_Loalphabet[ch];

			if (isCapital) {
				if (alphaFlags[ch] & 2)
					ch -= 0x20;
				isCapital = 0;
			}
			return ch;
		}
	}
			
	return ch;	
}

/********************************/
/*				*/
/* Opcode functions		*/
/*				*/
/********************************/

/*
 * _op_getElse()
 */
static void _op_getElse(FILE *fp, btstring_t *code, uint16_t *offset)
{
	uint16_t	rval;
	uint8_t		savedBreakAfter	= op_breakAfter;

	fprintf(fp, "\t\tthen ");
	if (!op_breakAfter) {
		fprintf(fp, "Do nothing");
		op_breakAfter	= 1;
	} else {
		rval	= str_read16le(&code->buf[*offset]);
		*offset	+= sizeof(uint16_t);

		fprintf(fp, "goto %04x", rval);
	}
	fprintf(fp, "\n\t\telse ");
	if (!savedBreakAfter) {
		fprintf(fp, " return");
	} else {
		fprintf(fp, "Do nothing");
	}
}

/*
 * 0 - op_stairsDown()
 */
static void op_stairsDown(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo stairs down");
}

/*
 * 1 - op_stairsUp()
 */
static void op_stairsUp(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo stairs up");
}

/*
 * 3 - op_teleport()
 */
static void op_teleport(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo teleport to (");
	printArgs(fp, code, offset, "b");
	fprintf(fp, ", ");
	printArgs(fp, code, offset, "b");
	fprintf(fp, ") in ");
	printArgs(fp, code, offset, "b");
}

/*
 * 5 - op_clearPrint()
 */
static void op_clearPrint(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo clearPrint ");
	printArgs(fp, code, offset, "s");
}

/*
 * 6 - op_clearSpecial()
 */
static void op_clearSpecial(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo clear this special");
}

/*
 * 7 - op_bigpic()
 */
static void op_bigpic(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo draw picture ");
	printArgs(fp, code, offset, "B");
}

/*
 * 8 - op_setTitle()
 */
static void op_setTitle(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo print ");
	printArgs(fp, code, offset, "S");
	fprintf(fp, " under picture");
}

/*
 * 9 - op_anyKey()
 */
static void op_anyKey(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo press any key");
}

/*
 * 10 - op_clear()
 */
static void op_clear(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo clear");
}
/*
 * 12 - op_isGFlagSet()
 */
static void op_isGFlagSet(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tIf global flag ");
	printArgs(fp, code, offset, "f");
	fprintf(fp, " is set\n");
	_op_getElse(fp, code, offset);
}


/*
 * 11 - op_isGFlagClear()
 */
static void op_isGFlagClear(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tIf global flag ");
	printArgs(fp, code, offset, "f");
	fprintf(fp, " is clear\n");
	_op_getElse(fp, code, offset);
}

/*
 * 14 - op_setGFlag()
 */
static void op_setGFlag(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tSet global flag ");
	printArgs(fp, code, offset, "f");
}

/*
 * 16 - op_ifSpellEq()
 */
static void op_ifSpellEq(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tIf spell = ");
	printArgs(fp, code, offset, "C");
	fprintf(fp, "\n");
	_op_getElse(fp, code, offset);
}

/*
 * 17 - op_setMapRval()
 */
static void op_setMapRval(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo set map rval");
}

/*
 * 18 - op_print()
 */
static void op_print(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo print ");
	printArgs(fp, code, offset, "s");
}

/*
 * 19 - op_nothing()
 */
static void op_nothing(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo nothing");
}

/*
 * 21 - op_getItem()
 */
static void op_getItem(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo give ");
	printArgs(fp, code, offset, "bIx");
}

/*
 * 22 - op_partyHasItem()
 */
static void op_partyHasItem(FILE *fp, btstring_t *code,  uint16_t *offset)
{
	fprintf(fp, "\tIf anyone is carrying ");
	printArgs(fp, code, offset, "I");
	fprintf(fp, "\n");
	_op_getElse(fp, code, offset);
}

/*
 * 24 - op_sameSquare()
 */
static void op_sameSquare(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tIf sameSquareFlag is set\n");
	_op_getElse(fp, code, offset);
}

/*
 * 25 - op_getYesNo()
 */
static void op_getYesNo(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tIf getYesNo is yes\n");
	_op_getElse(fp, code, offset);
}

/*
 * 26 - op_goto()
 */
static void op_goto(FILE *fp, btstring_t *code, uint16_t *offset)
{
	uint16_t	dest;

	dest	= str_read16le(&code->buf[*offset]);
	*offset	+= 2;

	fprintf(fp, "\tDo goto %04x", dest);
}

/*
 * 27 - op_battleNoCry()
 */
static void op_battleNoCry(FILE *fp, btstring_t *code, uint16_t *offset)
{
	uint8_t		numGroups;
	uint8_t		i;

	numGroups	= code->buf[(*offset)++];

	for (i = 0; i < numGroups; i++) {
		fprintf(fp, "\tDo set encounter: ");
		printArgs(fp, code, offset, "M");
		fprintf(fp, ":");
		printArgs(fp, code, offset, "b");
		fprintf(fp, "\n");
		
	}

	fprintf(fp, "\tDo begin combat\n");
	fprintf(fp, "\tIf party won last combat\n");
	_op_getElse(fp, code, offset);
}

/*
 * 28 - op_setSameSquare()
 */
static void op_setSameSquare(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo set sameSquareFlag");
}

/*
 * 43 - op_getInput()
 */
static void op_getInput(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo get input");
}

/*
 * 44 - op_ifLastInput()
 */
static void op_ifLastInput(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tIf last input = ");
	printArgs(fp, code, offset, "S");
	fprintf(fp, "\n");
	_op_getElse(fp, code, offset);
}

/*
 * 54 - op_userHasItem()
 */
static void op_userHasItem(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tIf user has item ");
	printArgs(fp, code, offset, "I");
	fprintf(fp, "\n");
	_op_getElse(fp, code, offset);
}

/*
 * 55 - op_takeItem()
 */
static void op_takeItem(FILE *fp, btstring_t *code, uint16_t *offset)
{
	fprintf(fp, "\tDo take item");
}

/********************************/
/*				*/
/* Disassembly instructions	*/
/*				*/
/********************************/

/*
 * disasmAction()
 */
static uint32_t disasmAction(FILE *fp, btstring_t *code, uint16_t *offset)
{
	uint8_t		opcode;
	uint8_t		rval		= 1;
	uint8_t		returnAfter	= 0;

	opcode = code->buf[*offset];
	returnAfter = !(opcode & 0x80);
	opcode &= 0x7f;
	(*offset)++;

	switch (opcode) {
	case 0:
		fprintf(fp, "Stairs Down");
		break;
	case 1:
		fprintf(fp, "Stairs Up");
		break;
	case 5:		/* ClearPrint */
		fprintf(fp, "clearPrint ");
		printArgs(fp, code, offset, "s");
		break;
	case 6:
		fprintf(fp, "Clear this special");
		break;
	case 7:
		fprintf(fp, "Draw Picture ");
		printArgs(fp, code, offset, "B");
		break;
	case 8:
		fprintf(fp, "Print ");
		printArgs(fp, code, offset, "S");
		fprintf(fp, " under picture");
		break;
	case 9:
		fprintf(fp, "Press Any Key");
		break;
	case 10:
		fprintf(fp, "Clear Text");
		break;
	case 11:
		fprintf(fp, "local flag ");
		printArgs(fp, code, offset, "b");
		fprintf(fp, " is set");
		break;
	case 12:
		fprintf(fp, "local flag ");
		printArgs(fp, code, offset, "f");
		fprintf(fp, " is not set");
		break;
	case 18:	/* Print */
		fprintf(fp, "Print ");
		printArgs(fp, code, offset, "s");
		break;
	case 19:
		fprintf(fp, "Nothing ");
		break;
		
	}

	return returnAfter;
}

/*
 * disasmOpcode()
 */
static void disasmOpcode(FILE *fp, btstring_t *code, uint16_t *offset)
{
	uint8_t		rval;
	uint8_t		opcode;

	opcode		= code->buf[(*offset)++];
/*	op_breakAfter	= !(opcode & 0x80);*/
	op_breakAfter	= opcode & 0x80;
	opcode		&= 0x7f;

	if (opcodes[opcode]) {
		opcodes[opcode](fp, code, offset);
	} else {
		fprintf(fp, "\topcode: %2d", opcode);
	}
}

/*
 * disasmSpecial()
 */
static void disasmSpecial(FILE *fp, btstring_t *code, uint16_t base, 
				uint16_t offset)
{
	uint8_t		opcode;
	uint32_t	doReturn;
	uint32_t	lineNumber	= 1;

	while (1) {
		fprintf(fp, "%04x:", offset + base);
/*		fprintf(fp, "%04x:", offset);*/
		disasmOpcode(fp, code, &offset);
		fprintf(fp, "\n");

#if 0
		if (opcodes[opcode]) {
			opcodes[opcode](fp, code, &offset);
			fprintf(fp, "\n");
		}
		else {
			fprintf(fp, "%2d\n", opcode);
			return;
		}

		fprintf(fp, "opcode: %d\n", opcode & 0x7f);
		if (isAction[opcode & 0x7f]) {
			fprintf(fp, "%d DO ", lineNumber);
			rval = disasmAction(fp, code, &offset);
			fprintf(fp, "\n");
		} else {
			fprintf(fp, "%d IF ", lineNumber);
			rval = disasmAction(fp, code, &offset);
			fprintf(fp, "\nTHEN ");

			/* Read offset of branch and then jump to it */
			disasmAction(fp, code, &offset);
			fprintf(fp, "\nELSE ");

			/* The other branch starts after the offset */
			disasmAction(fp, code, &offset);
			fprintf(fp, "\n");
		}
#endif

		if (doReturn)
			return;

		lineNumber++;
	}
}

/*
 * disasmCode()
 */
static void disasmCode(FILE *fp, btstring_t *code, uint16_t base)
{
	uint16_t	offset = 0;
	uint32_t	i;

	while (offset < code->size) {
		fprintf(fp, "%04x:", offset + base);
		disasmOpcode(fp, code, &offset);
		fprintf(fp, "\n");
		if (!op_breakAfter) 
			fprintf(fp, "\tReturn\n");
	}
}

/*
 * dump_disasm()
 */
static void dump_disasm(uint8_t dunno, uint8_t levno)
{
	uint32_t	i;
	b3level_t	*level;
	FILE		*fp;
	uint16_t	offset;

	level = readMap(duns[dunno].levels[levno]);
	currentLevel = level;

	fp = xfopen(mkCodePath("%s-%d.code", duns[dunno].name, levno), "wb");

#ifdef DUMP_CODE
	dump_btstring(bts_sprintf("%s-%d.dump", duns[dunno].name, levno), 
		level->codeBuf, 0);
#endif

#if DUMP_BY_SQUARE
	for (i = 0; i < level->dataCount; i++) {
/*		offset = level->dataList[i].offset - level->dataBaseOffset;*/
		offset = level->dataList[i].offset;
		fprintf(fp, "(%d, %d) offset: %04x\n",
			level->dataList[i].sqE,
			level->dataList[i].sqN,
			offset
			);
		offset = level->dataList[i].offset - level->dataBaseOffset;


		disasmSpecial(fp, level->codeBuf, level->dataBaseOffset, offset);
	}
#else
	for (i = 0; i < level->dataCount; i++) {
		offset = level->dataList[i].offset;
		fprintf(fp, "(%3d, %3d) offset: %04x\n",
			level->dataList[i].sqE,
			level->dataList[i].sqN,
			offset
			);
	}
	fprintf(fp, "=====================================\n");

	disasmCode(fp, level->codeBuf, level->dataBaseOffset);
#endif


	fclose(fp);

	freeLevel(level);
	currentLevel = NULL;
}

/********************************/
/*				*/
/* Public functions		*/
/*				*/
/********************************/

int main(int argc, char *argv[])
{
	extern int	optind;
	extern char	*optarg;
	int		c;

	uint32_t	dunno, levno;

	while ((c = getopt(argc, argv, "d:ho:")) != EOF) {
		switch (c) {
		case 'd':
			inputDir = bts_strcpy(optarg);
			break;
		case 'h':
			usage();
			exit(0);
		case 'o':
			outputDir = bts_strcpy(optarg);
			break;
		default:
			usage();
			exit(1);
		}
	}

	if (inputDir == NULL) {
		fprintf(stderr, "Input directory not specified\n");
		usage();
		return 1;
	}

	if (outputDir == NULL) {
		fprintf(stderr, "Output directory not specified\n");
		usage();
		return 1;
	}

	xmkdir(bts_sprintf("%s/bt3", outputDir->buf));
	xmkdir(bts_sprintf("%s/bt3/code", outputDir->buf));

#ifdef ONE_LEVELS
	dunno = 0;
	while (duns[dunno].name != NULL) {
		levno = 0;
		while (duns[dunno].levels[levno] != 0xff) {
			dump_disasm(dunno, levno);

			levno++;
		}
		dunno++;
	}
#else
	dump_disasm(1, 0);
#endif

	return 0;

}
