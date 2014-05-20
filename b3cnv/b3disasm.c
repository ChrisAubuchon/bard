#include <b3lib.h>
#include <dun.h>

#define DEBUG 1
#include <debug.h>

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
/* Local variables		*/
/*				*/
/********************************/

/* Packed string bits left */
static int8_t	sp_bitsLeft;

static uint8_t isAction[] = {
/*   0*/	1, 1, 1, 1, 
/*   4*/	1, 1, 1, 1, 
/*   8*/	1, 1, 1, 1, 
/*  12*/	0, 1, 1, 1, 
/*  16*/	1, 1, 1, 1, 
/*  20*/	1, 1, 1, 1, 
/*  24*/	1, 1, 1, 1, 
/*  28*/	1, 1, 1, 1, 
/*  32*/	1, 1, 1, 1, 
/*  36*/	1, 1, 1, 1, 
/*  40*/	1, 1, 1, 1, 
/*  44*/	1, 1, 1, 1, 
/*  48*/	1, 1, 1, 1, 
/*  52*/	1, 1, 1, 1, 
/*  56*/	1, 1, 1, 1, 
/*  60*/	1, 1, 1, 1, 
/*  64*/	1, 1, 1, 1, 
/*  68*/	1, 1, 1, 1, 
/*  72*/	1, 1, 1, 1, 
/*  76*/	1, 1, 1, 1, 
/*  80*/	1, 1, 1, 1, 
/*  84*/	1, 1, 1, 1, 
/*  88*/	1, 1, 1, 1, 
/*  92*/	1, 1, 1, 1, 
/*  96*/	1, 1, 1, 1, 
/* 100*/	1, 1, 1, 1, 
/* 104*/	1, 1, 1, 1, 
/* 108*/	1, 1, 1, 1, 
/* 112*/	1, 1, 1, 1
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
static void	disasmSpecial(FILE *, btstring_t *, uint16_t);
static uint32_t	disasmConditional(FILE *, btstring_t *, uint16_t *);
static uint32_t	disasmIf(FILE *, btstring_t *, uint16_t *);

static void	printArgs(FILE *, btstring_t *, uint16_t *, const char *);

static void	printPackedString(FILE *, btstring_t *, uint16_t *);
static void	printMaskedString(FILE *, btstring_t *, uint16_t *);
static void	printBigpic(FILE *, uint8_t);
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
			switch(format[i]) {
			case 'B':	/* Bigpic image */
				printBigpic(fp, code->buf[(*offset)++]);
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
 * disasmSpecial()
 */
static void disasmSpecial(FILE *fp, btstring_t *code, uint16_t offset)
{
	uint8_t		opcode;
	uint32_t	rval;
	uint32_t	lineNumber	= 1;

	while (1) {
		opcode	= code->buf[offset];
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

		if (rval)
			return;

		lineNumber++;
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

	fp = xfopen(mkCodePath("%s-%d.code", duns[dunno].name, levno), "wb");

	for (i = 0; i < level->dataCount; i++) {
		offset = level->dataList[i].offset - level->dataBaseOffset;
		fprintf(fp, "(%d, %d) offset: %04x\n",
			level->dataList[i].sqE,
			level->dataList[i].sqN,
			offset
			);


		disasmSpecial(fp, level->codeBuf, offset);
	}

	fclose(fp);

	freeLevel(level);
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
	dump_disasm(8, 0);
#endif

	return 0;

}
