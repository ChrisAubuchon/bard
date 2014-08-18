#include <b3lib.h>
#include <dun.h>
#include <gl_xlist.h>
#include <gl_array_list.h>

#define DEBUG 1
#include <debug.h>

/********************************/
/*				*/
/* Internal Types		*/
/*				*/
/********************************/


typedef struct {
	FILE		*fp;
	btstring_t	*code;
	uint16_t	offset;
} disasm_t;

typedef struct {
	uint16_t	start;
	uint16_t	end;
} range_t;

typedef struct {
	uint16_t	codeStartOffset;
	uint8_t		monsterIndex;
	uint16_t	dataStartOffset;
} ilevel_t;

typedef void (opcodeFunc_t)(disasm_t *);

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

static void	op_doConditional(disasm_t *);

static void	op_stairsDown(disasm_t *);
static void	op_stairsUp(disasm_t *);
static void	op_misc(disasm_t *);
static void	op_teleport(disasm_t *);
static void	op_battle(disasm_t *);
static void	op_clearPrint(disasm_t *);
static void	op_clearSpecial(disasm_t *);
static void	op_bigpic(disasm_t *);
static void	op_setTitle(disasm_t *);
static void	op_anyKey(disasm_t *);
static void	op_clear(disasm_t *);
static void	op_isGFlagSet(disasm_t *);
static void	op_isGFlagClear(disasm_t *);
static void	op_makeDoor(disasm_t *);
static void	op_setGFlag(disasm_t *);
static void	op_clearGFlag(disasm_t *);
static void	op_ifSpellEq(disasm_t *);
static void	op_setMapRval(disasm_t *);
static void	op_print(disasm_t *);
static void	op_nothing(disasm_t *);
static void	op_ifLiquid(disasm_t *);
static void	op_getItem(disasm_t *);
static void	op_partyHasItem(disasm_t *);
static void	op_partyNotHasItem(disasm_t *);
static void	op_sameSquare(disasm_t *);
static void	op_getYesNo(disasm_t *);
static void	op_goto(disasm_t *);
static void	op_battleNoCry(disasm_t *);
static void	op_setSameSquare(disasm_t *);
static void	op_turnParty(disasm_t *);
static void	op_rmItem(disasm_t *);
static void	op_decLFlag(disasm_t *);
static void	op_ifLFlag(disasm_t *);
static void	op_ifInBox(disasm_t *);
static void	op_drainHp(disasm_t *);
static void	op_setLiquid(disasm_t *);
static void	op_addToInv(disasm_t *);
static void	op_subFromInv(disasm_t *);
static void	op_setDirection(disasm_t *);
static void	op_addToLFlag(disasm_t *);
static void	op_getInput(disasm_t *);
static void	op_ifLastInput(disasm_t *);
static void	op_readNumber(disasm_t *);
static void	op_getCharacter(disasm_t *);
static void	op_ifRegLt(disasm_t *);
static void	op_ifRegEq(disasm_t *);
static void	op_ifGiveGold(disasm_t *);
static void	op_learnSpell(disasm_t *);
static void	op_setLFlag(disasm_t *);
static void	op_userHasItem(disasm_t *);
static void	op_takeItem(disasm_t *);
static void	op_addMonster(disasm_t *);
static void	op_ifMonInParty(disasm_t *);
static void	op_clearPrintOffset(disasm_t *);
static void	op_ifIsNight(disasm_t *);
static void	op_rmMonFromParty(disasm_t *);
static void	op_setQuestFlag(disasm_t *);
static void	op_isSqFlagSet(disasm_t *);
static void	op_setSqFlag(disasm_t *);
static void	op_ifIsClass(disasm_t *);
static void	op_printOffset(disasm_t *);
static void	op_clearTeleport(disasm_t *);

/********************************/
/*				*/
/* Local variables		*/
/*				*/
/********************************/

/* List of offset ranges to skip */
static gl_list_t	rangeSkipList;


/* Packed string bits left */
static int8_t	sp_bitsLeft;

/* Opcode breakAfter */
static uint8_t	op_breakAfter;

/* Global level data */
static ilevel_t		currentLevel;

static opcodeFunc_t *opcodes[] = {
/*   0*/	op_stairsDown, op_stairsUp, op_misc, op_teleport, 
/*   4*/	op_battle, op_clearPrint, op_clearSpecial, op_bigpic, 
/*   8*/	op_setTitle, op_anyKey, op_clear, op_isGFlagSet, 
/*  12*/	op_isGFlagClear, op_makeDoor, op_setGFlag, op_clearGFlag, 
/*  16*/	op_ifSpellEq, op_setMapRval, op_print, op_nothing, 
/*  20*/	op_ifLiquid, op_getItem, op_partyHasItem, op_partyNotHasItem, 
/*  24*/	op_sameSquare, op_getYesNo, op_goto, op_battleNoCry, 
/*  28*/	op_setSameSquare, op_turnParty, op_rmItem, NULL, 
/*  32*/	op_decLFlag, op_ifLFlag, NULL, op_drainHp, 
/*  36*/	op_ifInBox, op_setLiquid, op_addToInv, op_subFromInv, 
/*  40*/	op_addToLFlag, NULL, op_setDirection, op_getInput, 
/*  44*/	op_ifLastInput, op_readNumber, op_getCharacter, op_ifGiveGold, 
/*  48*/	NULL, op_ifRegLt, op_ifRegEq, NULL, 
/*  52*/	op_learnSpell, op_setLFlag, op_userHasItem, op_takeItem, 
/*  56*/	op_addMonster, op_ifMonInParty, op_clearPrintOffset, op_ifIsNight, 
/*  60*/	op_rmMonFromParty, NULL, NULL, op_setQuestFlag, 
/*  64*/	NULL, NULL, op_isSqFlagSet, op_setSqFlag, 
/*  68*/	op_ifIsClass, op_printOffset, op_clearTeleport, NULL, 
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
static void	level_disasm(uint8_t, uint8_t);
static void	city_disasm(uint8_t);
static void	disasmCode(FILE *, btstring_t *, uint16_t);
static uint32_t	disasmConditional(disasm_t *);
static void	disasmOpcode(disasm_t *, uint16_t);
static uint32_t	disasmIf(disasm_t *);

static void	oprintf(disasm_t *, const char *);

static void	printPackedString(disasm_t *);
static void	printMaskedString(disasm_t *);
static void	printBigpic(FILE *, uint8_t);
static void	printSpell(FILE *, uint8_t);
static void	printItem(FILE *, uint8_t);
static void	printMonster(FILE *, uint8_t);
static void	printClass(disasm_t *);
static void	printLiquid(FILE *, uint8_t);
static uint8_t	sp_unpackChar(disasm_t *);
static uint8_t	sp_extractCh(disasm_t *, uint8_t);

static uint32_t	isSkipped(disasm_t *);
static void	range_free(const void *);

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

/*
 * range_free()
 */
static void range_free(const void *vr)
{
	range_t		*r = (range_t *)vr;

	free(r);
}

/*
 * isSkipped()
 */
static uint32_t isSkipped(disasm_t *d)
{
	size_t		listSize;
	range_t		*r;
	uint32_t	i;

	listSize	= gl_list_size(rangeSkipList);

	if (!listSize)
		return 0;

	for (i = 0; i < listSize; i++) {
		r = (range_t *)gl_list_get_at(rangeSkipList, i);

		if ((d->offset >= r->start) && (d->offset < r->end))
			return 1;
	}

	return 0;
}

/********************************/
/*				*/
/* Print Functions		*/
/*				*/
/********************************/

/*
 * oprintf()
 *
 * Opcode printf
 */
static void oprintf(disasm_t *d, const char *format)
{
	uint32_t	i	= 0;
	const char 	*fstart, *fend;

	if (format == NULL)
		return;

	fstart	= fend	= format;

	while (*fend) {
		if (*fend != '%') {
			i++;
			fend++;
		} else {
			if (i) 
				fprintf(d->fp, "%.*s", i, fstart);

			/* Skip past % */
			fend++;

			switch (*fend) {
			case 'b':
				fprintf(d->fp, "%d", d->code->buf[d->offset++]);
			break;
			case 'B':	/* Bigpic image */
				printBigpic(d->fp, d->code->buf[d->offset++]);
				break;
			case 'c':	/* Class */
				printClass(d);
				break;
			case 'C':	/* Spell */
				printSpell(d->fp, d->code->buf[d->offset++]);
				break;
			case 'D':	/* Direction */
			{
				uint8_t		dir;

				dir = d->code->buf[d->offset++];

				if (dir == 0) {
					fprintf(d->fp, "north");
				} else if (dir == 1) {
					fprintf(d->fp, "east");
				} else if (dir == 2) {
					fprintf(d->fp, "south");
				} else if (dir == 3) {
					fprintf(d->fp, "west");
				} else {
					fprintf(d->fp, "unknown");
				}
				break;
			}
			case 'f':	/* Flag */
			{
				uint8_t		flag;
				uint8_t		mask;

				flag	= d->code->buf[d->offset] >> 3;
				mask	= d->code->buf[d->offset] & 7;
				fprintf(d->fp, "%d", (flag * 8) + mask);
				d->offset++;
				break;
			}
			case 'I':	/* Item */
				printItem(d->fp, d->code->buf[d->offset++]);
				break;
			case 'L':
				printLiquid(d->fp, d->code->buf[d->offset++]);
				break;
			case 'M':	/* Monster */
				printMonster(d->fp, d->code->buf[d->offset++]);
				break;
			case 'O':	/* String at offset */
			{
				uint16_t	savedOffset;
				range_t		*r;

				r = (range_t *)xzalloc(sizeof(range_t));

				savedOffset = d->offset + 2;

				d->offset = str_read16le(&d->code->buf[d->offset]) - currentLevel.dataStartOffset;
				r->start	= d->offset;
				printPackedString(d);
				r->end		= d->offset;
				gl_list_add_last(rangeSkipList, r);
				d->offset	= savedOffset;
				break;
			}
			case 'o':	/* offset */
				fprintf(d->fp, "%04x",
					str_read16le(&d->code->buf[d->offset])
					);
				d->offset += 2;
				break;
			case 'S':	/* Masked String */
				fprintf(d->fp, "\"");
				fflush(d->fp);
				printMaskedString(d);
				fprintf(d->fp, "\"");
				break;
			case 's':	/* Packed String */
				fprintf(d->fp, "\"");
				printPackedString(d);
				fprintf(d->fp, "\"");
				break;
			case 'w':	/* Word */
				fprintf(d->fp, "%d", 
					str_read16le(&d->code->buf[d->offset])
					);
				d->offset	+= 2;
				break;
			case 'x':	/* Hex byte */
				fprintf(d->fp, "0x%02x", d->code->buf[d->offset++]);
				break;
			}

			fend++;
			fstart	= fend;
			i	= 0;
		}
	}

	if (i)
		fprintf(d->fp, "%.*s", i, fstart);
}


/*
 * printPackedString()
 */
static void printPackedString(disasm_t *d)
{
	btstring_t	*rval;
	uint32_t	stri	= 0;

	sp_bitsLeft = 0;

	rval	= bts_new(1024);

	while (rval->buf[stri++] = sp_unpackChar(d));

	bts_resize(rval, stri);

	fprintf(d->fp, "%.*s", rval->size, rval->buf);

	bts_free(rval);
}

/*
 * printMaskedString()
 */
static void printMaskedString(disasm_t *d)
{
	btstring_t	*rval;
	uint32_t	stri	= 0;

	rval = bts_new(1024);

	while (d->code->buf[d->offset] != 0xff) 
		rval->buf[stri++] = d->code->buf[d->offset++] & 0x7f;

	rval->buf[stri++] = '\0';
	d->offset++;

	bts_resize(rval, stri);

	fprintf(d->fp, "%.*s", rval->size, rval->buf);

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
	} else if (spell == 128) {
		fprintf(fp, "eat_acorn");
	} else if (spell == 129) {
		fprintf(fp, "use_wineskin");
	} else if (spell == 130) {
		fprintf(fp, "use_item");
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

	rval	= getLevelMonster(currentLevel.monsterIndex, monster);

	fprintf(fp, "%.*s", rval->size, rval->buf);

	bts_free(rval);
}

/*
 * printLiquid()
 */
static void printLiquid(FILE *fp, uint8_t liquid)
{
	btstring_t	*rval;

	rval	= getLiquid(liquid);

	fprintf(fp, "%.*s", rval->size, rval->buf);

	bts_free(rval);
}


/*
 * printClass()
 */
static void printClass(disasm_t *d)
{
	btstring_t	*rval;

	rval = getClass(d->code->buf[d->offset++]);

	fprintf(d->fp, "%.*s", rval->size, rval->buf);

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
static uint8_t sp_extractCh(disasm_t *d, uint8_t count)
{
	uint8_t		rval = 0;

	static uint8_t	curByte;

	while (count--) {
		sp_bitsLeft--;
		if (sp_bitsLeft < 0) {
			curByte = d->code->buf[d->offset++];
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
static uint8_t sp_unpackChar(disasm_t *d)
{
	uint8_t		ch;
	uint8_t		isCapital	= 0;

	while (1) {
		ch = sp_extractCh(d, 5);
		if (!ch)
			break;

		if (ch == 30) {
			isCapital = 1;
		} else {
			if (ch == 31)
				ch = _str_Hialphabet[
					sp_extractCh(d, 6)];
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
 * op_doConditional()
 */
static void op_doConditional(disasm_t *d)
{
	uint16_t	rval;
	uint8_t		savedBreakAfter	= op_breakAfter;

	fprintf(d->fp, "\t\tthen ");
	if (!op_breakAfter) {
		fprintf(d->fp, "Do nothing");
		op_breakAfter	= 1;
	} else {
		rval	= str_read16le(&d->code->buf[d->offset]);
		d->offset	+= sizeof(uint16_t);

		fprintf(d->fp, "goto %04x", rval);
	}
	fprintf(d->fp, "\n\t\telse ");
	if (!savedBreakAfter) {
		fprintf(d->fp, " return");
	} else {
		fprintf(d->fp, "Do nothing");
	}
}

/*
 * 0 - op_stairsDown()
 */
static void op_stairsDown(disasm_t *d)
{
	fprintf(d->fp, "\tDo stairs down");
}

/*
 * 1 - op_stairsUp()
 */
static void op_stairsUp(disasm_t *d)
{
	fprintf(d->fp, "\tDo stairs up");
}

/*
 * 2 - op_misc()
 *
 * Do a different action based on the byte argument
 */
static void op_misc(disasm_t *d)
{
	uint8_t		subOp;

	subOp		= d->code->buf[d->offset++];

	switch (subOp) {
	case 3:
		oprintf(d, "\tDo Convert to Geomancer");
		break;
	case 4:
		oprintf(d, "\tDo SCSI");
		break;
	case 5:
		oprintf(d, "\tDo Victory");
		break;
	case 9:
		oprintf(d, "\tDo Copy Protection");
		break;
	}
}

/*
 * 3 - op_teleport()
 */
static void op_teleport(disasm_t *d)
{
	oprintf(d, "\tDo teleport to (%b, %b) in %b");
}

/*
 * 4 - op_battle()
 */
static void op_battle(disasm_t *d)
{
	uint8_t		numGroups;
	uint8_t		i;

	numGroups	= d->code->buf[d->offset++];

	for (i = 0; i < numGroups; i++) {
		oprintf(d, "\tDo set encounter: %M:%b\n");
	}
		
	fprintf(d->fp, "\tDo begin combat\n");
	fprintf(d->fp, "\tIf party won last combat\n");
	op_doConditional(d);
}
/*
 * 5 - op_clearPrint()
 */
static void op_clearPrint(disasm_t *d)
{
	oprintf(d, "\tDo clearPrint %s");
}

/*
 * 6 - op_clearSpecial()
 */
static void op_clearSpecial(disasm_t *d)
{
	oprintf(d, "\tDo clear special at %o");
}

/*
 * 7 - op_bigpic()
 */
static void op_bigpic(disasm_t *d)
{
	oprintf(d, "\tDo draw picture %B");
}

/*
 * 8 - op_setTitle()
 */
static void op_setTitle(disasm_t *d)
{
	oprintf(d, "\tDo print %S under picture");
}

/*
 * 9 - op_anyKey()
 */
static void op_anyKey(disasm_t *d)
{
	fprintf(d->fp, "\tDo press any key");
}

/*
 * 10 - op_clear()
 */
static void op_clear(disasm_t *d)
{
	fprintf(d->fp, "\tDo clear");
}
/*
 * 12 - op_isGFlagSet()
 */
static void op_isGFlagSet(disasm_t *d)
{
	oprintf(d, "\tIf global flag %f is set\n");
	op_doConditional(d);
}

/*
 * 13 - op_makeDoor()
 */
static void op_makeDoor(disasm_t *d)
{
	oprintf(d, "\tDo make door at row %b, square %b, %x");
}


/*
 * 11 - op_isGFlagClear()
 */
static void op_isGFlagClear(disasm_t *d)
{
	oprintf(d, "\tIf global flag %f is clear\n");
	op_doConditional(d);
}

/*
 * 14 - op_setGFlag()
 */
static void op_setGFlag(disasm_t *d)
{
	oprintf(d, "\tSet global flag %f");
}

/*
 * 15 - op_clearGFlag()
 */
static void op_clearGFlag(disasm_t *d)
{
	oprintf(d, "\tClear global flag %f");
}

/*
 * 16 - op_ifSpellEq()
 */
static void op_ifSpellEq(disasm_t *d)
{
	oprintf(d, "\tIf spell = %C\n");
	op_doConditional(d);
}

/*
 * 17 - op_setMapRval()
 */
static void op_setMapRval(disasm_t *d)
{
	fprintf(d->fp, "\tDo set map rval");
}

/*
 * 18 - op_print()
 */
static void op_print(disasm_t *d)
{
	oprintf(d, "\tDo print %s");
}

/*
 * 19 - op_nothing()
 */
static void op_nothing(disasm_t *d)
{
	fprintf(d->fp, "\tDo nothing");
}

/*
 * 20 - op_ifLiquid()
 */
static void op_ifLiquid(disasm_t *d)
{
	oprintf(d, "\tIf liquid = %L\n");
	op_doConditional(d);
}

/*
 * 21 - op_getItem()
 */
static void op_getItem(disasm_t *d)
{
	oprintf(d, "\tDo give %b %I %x");
}

/*
 * 22 - op_partyHasItem()
 */
static void op_partyHasItem(disasm_t *d)
{
	oprintf(d, "\tIf anyone is carrying %I\n");
	op_doConditional(d);
}

/*
 * 23 - op_partyNotHasItem()
 */
static void op_partyNotHasItem(disasm_t *d)
{
	oprintf(d, "\tIf noone is carrying %I\n");
	op_doConditional(d);
}
/*
 * 24 - op_sameSquare()
 */
static void op_sameSquare(disasm_t *d)
{
	fprintf(d->fp, "\tIf sameSquareFlag is set\n");
	op_doConditional(d);
}

/*
 * 25 - op_getYesNo()
 */
static void op_getYesNo(disasm_t *d)
{
	fprintf(d->fp, "\tIf getYesNo is yes\n");
	op_doConditional(d);
}

/*
 * 26 - op_goto()
 */
static void op_goto(disasm_t *d)
{
	uint16_t	dest;

	dest	= str_read16le(&d->code->buf[d->offset]);
	d->offset	+= 2;

	fprintf(d->fp, "\tDo goto %04x", dest);
}

/*
 * 27 - op_battleNoCry()
 */
static void op_battleNoCry(disasm_t *d)
{
	uint8_t		numGroups;
	uint8_t		i;

	numGroups	= d->code->buf[d->offset++];

	for (i = 0; i < numGroups; i++) {
		oprintf(d, "\tDo set encounter: %M:%b\n");
		
	}

	fprintf(d->fp, "\tDo begin combat no cry\n");
	fprintf(d->fp, "\tIf party won last combat\n");
	op_doConditional(d);
}

/*
 * 28 - op_setSameSquare()
 */
static void op_setSameSquare(disasm_t *d)
{
	fprintf(d->fp, "\tDo set sameSquareFlag");
}

/*
 * 29 - op_turnParty()
 */
static void op_turnParty(disasm_t *d)
{
	fprintf(d->fp, "\tDo turn party around");
}

/*
 * 30 - op_rmItem()
 */
static void op_rmItem(disasm_t *d)
{
	oprintf(d, "\tDo remove item %I");
}

/*
 * 32 - op_decLFlag()
 */
static void op_decLFlag(disasm_t *d)
{
	oprintf(d, "\tDecrement local flag %b");
}

/*
 * 33 - op_ifLFlag()
 */
static void op_ifLFlag(disasm_t *d)
{
	oprintf(d, "\tIf local flag %b not zero\n");
	op_doConditional(d);
}

/*
 * 35 - op_drainHp()
 */
static void op_drainHp(disasm_t *d)
{
	oprintf(d, "\tDo drain hp %w");
}

/*
 * 36 - op_ifInBox()
 */
static void op_ifInBox(disasm_t *d)
{
	oprintf(d, "\tIf inBox %b, %b, %b, %b\n");
	op_doConditional(d);
}

/*
 * 37 - op_setLiquid()
 */
static void op_setLiquid(disasm_t *d)
{
	oprintf(d, "\tDo set liquid to %L");
}

/*
 * 38 - op_addToInv()
 */
static void op_addToInv(disasm_t *d)
{
	oprintf(d, "\tAdd to container %b");
}

/*
 * 39 - op_subFromInv()
 */
static void op_subFromInv(disasm_t *d)
{
	oprintf(d, "\tSubtract from container %b");
}

/*
 * 40 - op_addToLFlag()
 */
static void op_addToLFlag(disasm_t *d)
{
	oprintf(d, "\tDo add to local flag %b %w");
}

/*
 * 42 - op_setDirection()
 */
static void op_setDirection(disasm_t *d)
{
	oprintf(d, "\tDo set direction to %D");
}

/*
 * 43 - op_getInput()
 */
static void op_getInput(disasm_t *d)
{
	fprintf(d->fp, "\tDo get input");
}

/*
 * 44 - op_ifLastInput()
 */
static void op_ifLastInput(disasm_t *d)
{
	oprintf(d, "\tIf last input = %S\n");
	op_doConditional(d);
}

/*
 * 45 - op_readNumber()
 */
static void op_readNumber(disasm_t *d)
{
	oprintf(d, "\tRead number to LFlag %b");
}

/*
 * 46 - op_getCharacter()
 */
static void op_getCharacter(disasm_t *d)
{
	oprintf(d, "\t If get character then\n");
	op_doConditional(d);
}

/*
 * 47 - op_ifGiveGold()
 */
static void op_ifGiveGold(disasm_t *d)
{
	oprintf(d, "\tIf give gold %b then\n");
	op_doConditional(d);
}

/*
 * 49 - op_ifRegLt()
 */
static void op_ifRegLt(disasm_t *d)
{
	oprintf(d, "\tIf reg %b < %w then\n");
	op_doConditional(d);
}
/*
 * 50 - op_ifRegEq()
 */
static void op_ifRegEq(disasm_t *d)
{
	oprintf(d, "\tIf reg %b = %w then\n");
	op_doConditional(d);
}

/*
 * 52 - op_learnSpell()
 */
static void op_learnSpell(disasm_t *d)
{
	oprintf(d, "\tDo learn spell %C");
}

/*
 * 53 - op_setLFlag()
 */
static void op_setLFlag(disasm_t *d)
{
	oprintf(d, "\tSet LFlag %b %w");
}

/*
 * 54 - op_userHasItem()
 */
static void op_userHasItem(disasm_t *d)
{
	oprintf(d, "\tIf user used item %I\n");
	op_doConditional(d);
}

/*
 * 55 - op_takeItem()
 */
static void op_takeItem(disasm_t *d)
{
	fprintf(d->fp, "\tDo take item");
}

/*
 * 56 - op_addMonster()
 */
static void op_addMonster(disasm_t *d)
{
	oprintf(d, "\tIf add %M to party\n");
	op_doConditional(d);
}

/*
 * 57 - op_ifMonInParty()
 */
static void op_ifMonInParty(disasm_t *d)
{
	oprintf(d, "\tIf monster %S in party\n");
	op_doConditional(d);
}

/*
 * 58 - op_clearPrintOffset()
 */
static void op_clearPrintOffset(disasm_t *d)
{
	oprintf(d, "\tDo clear and print \"%O\"");
}

/*
 * 59 - op_ifIsNight()
 */
static void op_ifIsNight(disasm_t *d)
{
	oprintf(d, "\tIf Is Night then\n");
	op_doConditional(d);
}

/*
 * 60 - op_rmMonFromParty()
 */
static void op_rmMonFromParty(disasm_t *d)
{
	oprintf(d, "\tDo remove monster");
}

/*
 * 63 - op_setQuestFlag()
 */
static void op_setQuestFlag(disasm_t *d)
{
	oprintf(d, "\tDo set quest flag %b");
}

/*
 * 66 - op_isSqFlagSet()
 */
static void op_isSqFlagSet(disasm_t *d)
{
	oprintf(d, "\tIf Square Flag %b is set then\n");
	op_doConditional(d);
}

/*
 * 67 - op_setSqFlag()
 */
static void op_setSqFlag(disasm_t *d)
{
	oprintf(d, "\tDo Set Square Flag %b");
}

/*
 * 68 - op_ifIsClass()
 */
static void op_ifIsClass(disasm_t *d)
{
	oprintf(d, "\tIf is class %c then\n");
	op_doConditional(d);
}

/*
 * 69 - op_printOffset()
 */
static void op_printOffset(disasm_t *d)
{
	oprintf(d, "\tDo offset_print \"%O\"");
}

/*
 * 70 - op_clearTeleport()
 */
static void op_clearTeleport(disasm_t *d)
{
	oprintf(d, "\tDo clear and teleport to (%b, %b) in %b\n");
}


/********************************/
/*				*/
/* Disassembly instructions	*/
/*				*/
/********************************/

#if 0
/*
 * disasmAction()
 */
static uint32_t disasmAction(disasm_t *d)
{
	uint8_t		opcode;
	uint8_t		rval		= 1;
	uint8_t		returnAfter	= 0;

	opcode = d->code->buf[d->offset];
	returnAfter = !(opcode & 0x80);
	opcode &= 0x7f;
	d->offset++;

	switch (opcode) {
	case 0:
		fprintf(d->fp, "Stairs Down");
		break;
	case 1:
		fprintf(d->fp, "Stairs Up");
		break;
	case 5:		/* ClearPrint */
		fprintf(d->fp, "clearPrint ");
		printArgs(d, "s");
		break;
	case 6:
		fprintf(d->fp, "Clear this special");
		break;
	case 7:
		fprintf(d->fp, "Draw Picture ");
		printArgs(d, "B");
		break;
	case 8:
		fprintf(d->fp, "Print ");
		printArgs(d, "S");
		fprintf(d->fp, " under picture");
		break;
	case 9:
		fprintf(d->fp, "Press Any Key");
		break;
	case 10:
		fprintf(d->fp, "Clear Text");
		break;
	case 11:
		fprintf(d->fp, "local flag ");
		printArgs(d, "b");
		fprintf(d->fp, " is set");
		break;
	case 12:
		fprintf(d->fp, "local flag ");
		printArgs(d, "f");
		fprintf(d->fp, " is not set");
		break;
	case 18:	/* Print */
		fprintf(d->fp, "Print ");
		printArgs(d, "s");
		break;
	case 19:
		fprintf(d->fp, "Nothing ");
		break;
		
	}

	return returnAfter;
}
#endif

/*
 * disasmOpcode()
 */
static void disasmOpcode(disasm_t *d, uint16_t base)
{
	uint8_t		rval;
	uint8_t		opcode;

	if (isSkipped(d)) {
		d->offset++;
		return;
	}

	opcode		= d->code->buf[d->offset++];

	op_breakAfter	= opcode & 0x80;
	opcode		&= 0x7f;

	if (opcode == 127)
		return;

	fprintf(d->fp, "%04x:", d->offset + base - 1);

	if (opcodes[opcode]) {
		opcodes[opcode](d);
	} else {
		fprintf(d->fp, "\topcode: %2d", opcode);
	}
	fprintf(d->fp, "\n");
	if (!op_breakAfter) 
		fprintf(d->fp, "\tReturn\n");

	fflush(d->fp);
}

/*
 * disasmCode()
 */
static void disasmCode(FILE *fp, btstring_t *code, uint16_t base)
{
	disasm_t	d;
	uint32_t	i;

	d.fp		= fp;
	d.offset	= currentLevel.codeStartOffset;
	d.code		= code;

	while (d.offset < code->size) {
		disasmOpcode(&d, base);
	}
}

/*
 * level_disasm()
 */
static void level_disasm(uint8_t dunno, uint8_t levno)
{
	uint32_t	i;
	b3level_t	*level;
	FILE		*fp;
	uint16_t	offset;

	rangeSkipList = gl_list_create_empty(GL_ARRAY_LIST,
		NULL, NULL, range_free, 1);

	level = readMap(duns[dunno].levels[levno]);

	currentLevel.dataStartOffset	= level->dataBaseOffset;
	currentLevel.monsterIndex	= level->monIndex;

	if ((dunno == 11) && (levno == 0))
		currentLevel.codeStartOffset = 4;
	else
		currentLevel.codeStartOffset = 0;

	fp = xfopen(mkCodePath("%s-%d.code", duns[dunno].name, levno), "wb");

#undef DUMP_CODE
#ifdef DUMP_CODE
	dump_btstring(bts_sprintf("%s-%d.dump", duns[dunno].name, levno), 
		level->codeBuf, 0);
#endif

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

	gl_list_free(rangeSkipList);
	rangeSkipList = NULL;

	fclose(fp);

	freeLevel(level);
}

/*
 * city_disasm()
 */
static void city_disasm(uint8_t cityno)
{
	uint32_t	i;
	b3city_t	*city;
	FILE		*fp;
	uint16_t	offset;

	rangeSkipList = gl_list_create_empty(GL_ARRAY_LIST,
		NULL, NULL, range_free, 1);

	city	= readCity(cityno);

	currentLevel.dataStartOffset	= city->dataBaseOffset;
	currentLevel.monsterIndex	= city->monsterIndex;
	currentLevel.codeStartOffset	= 0;

	fp = xfopen(mkCodePath("%s.code", cityList[cityno].name, 0), "wb");

	for (i = 0; i < city->dataCount; i++) {
		offset	= city->dataList[i].offset;
		fprintf(fp, "(%3d, %3d) offset: %04x\n",
			city->dataList[i].sqE,
			city->dataList[i].sqN,
			offset
			);
	}

	fprintf(fp, "=====================================\n");

#undef DUMP_CODE
#ifdef DUMP_CODE
	dump_btstring(bts_sprintf("%s.dump", cityList[cityno].name), 
		city->codeBuf, 1);
#endif

	disasmCode(fp, city->codeBuf, city->dataBaseOffset);

	gl_list_free(rangeSkipList);
	rangeSkipList = NULL;

	fclose(fp);

	freeCity(city);
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

	dunno = 0;
	while (duns[dunno].name != NULL) {
		levno = 0;
		while (duns[dunno].levels[levno] != 0xff) {
			level_disasm(dunno, levno);

			levno++;
		}
		dunno++;
	}

	levno = 0;
	while (cityList[levno].name != NULL) {
		city_disasm(levno);
		levno++;
	}

	return 0;

}
