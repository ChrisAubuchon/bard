/*
 * picIndex.c - Picture index number functions
 */
#include <b3lib.h>
#include <fileio.h>


static uint8_t picTran[] = {
/*  0*/ 0, 1, 2, 3,
/*  4*/ 4, 5, 6, 7,
/*  8*/ 8, 9, 0x0A, 0x0B,
/* 12*/ 0x0C, 0x0D, 0x0E, 0x0F,
/* 16*/ 0x10, 0x11, 0x12, 0x13,
/* 20*/ 0x14, 0x15, 0xFF, 0xFF,
/* 24*/ 0x16, 0xFF, 0x17, 0xFF,
/* 28*/ 0xFF, 0x18, 0x19, 0xFF,
/* 32*/ 0x1A, 0x1B, 0xFF, 0xFF,
/* 36*/ 0xFF, 0xFF, 0x1C, 0x1D,
/* 40*/ 0xFF, 0xFF, 0xFF, 0x1E,
/* 44*/ 0xFF, 0xFF, 0x1F, 0x20,
/* 48*/ 0x21, 0x22, 0x23, 0x24,
/* 52*/ 0x25, 0x26, 0x27, 0x28,
/* 56*/ 0x29, 0x2A, 0x2B, 0x2C,
/* 60*/ 0xFF, 0x2D, 0xFF, 0x2E,
/* 64*/ 0xFF, 0xFF, 0xFF, 0xFF,
/* 68*/ 0xFF, 0x2F, 0x30, 0x31,
/* 72*/ 0xFF, 0xFF, 0x32, 0xFF,
/* 76*/ 0xFF, 0xFF, 0x33, 0x34,
/* 80*/ 0xFF, 0xFF, 0xFF, 0x35,
/* 84*/ 0xFF, 0xFF, 0xFF, 0xFF,
/* 88*/ 0xFF, 0xFF, 0xFF, 0,		/*   0*/
/* 92*/ 0xFF, 1, 0xFF, 2,		/*   4*/
/* 96*/ 0xFF, 3, 4, 0xFF,		/*   8*/
/*100*/ 0xFF, 0xFF, 5, 6,		/*  12*/
/*104*/ 0xFF, 0xFF, 0xFF, 0xFF,		/*  16*/
/*108*/ 0xFF, 7, 8, 9,			/*  20*/
/*112*/ 0xFF, 0x0A, 0xFF, 0xFF,		/*  24*/
/*116*/ 0x0B, 0xFF, 0xFF, 0x0C,		/*  28*/
/*120*/ 0x0D, 0xFF, 0x0E, 0x0F,		/*  32*/
/*124*/ 0x10, 0x11, 0x12, 0xFF,		/*  36*/
/*128*/ 0x13, 0x14, 0x15, 0xFF,		/*  40*/
/*132*/ 0x16, 0x17, 0xFF, 0xFF,		/*  44*/
/*136*/ 0x18, 0xFF, 0xFF, 0x19,		/*  48*/
/*140*/ 0x1A, 0x1B, 0x1C, 0x1D,		/*  52*/
/*144*/ 0xFF, 0xFF, 0xFF, 0xFF,		/*  56*/
/*148*/ 0x1E, 0x1F, 0x20, 0xFF,		/*  60*/
/*152*/ 0x21, 0x22, 0x23, 0x24,		/*  64*/
/*156*/ 0x25, 0xFF, 0xFF, 0xFF,		/*  68*/
/*160*/ 0x26, 0x27, 0xFF, 0x28,		/*  72*/
/*164*/ 0x29, 0x2A, 0x2B, 0x2C,		/*  76*/
/*168*/ 0x2D, 0xFF, 0x2E, 0xFF,		/*  80*/
/*172*/ 0x2F, 0x30, 0xFF, 0x31		/*  84*/
};


static uint8_t *picMac[] = {
/*  0*/  "PIC_CAMP", "PIC_VIPER", "PIC_HOOKFANG","PIC_L3",
/*  4*/  "PIC_HOUND", "PIC_L5", "PIC_IMP", "PIC_L7",
/*  8*/  "PIC_NEWT", "PIC_L9", "PIC_L10", "PIC_NEFAST",
/* 12*/  "PIC_GORGON", "PIC_PIRANHA", "PIC_L14", "PIC_L15",
/* 16*/  "PIC_PHANTOM", "PIC_BRILHASTI", "PIC_CORRUPTER", "PIC_PUMA",
/* 20*/  "PIC_ZEPHYR", "PIC_L21", "PIC_NONE", "PIC_NONE",
/* 24*/  "PIC_RAINBOWDRAGON", "PIC_NONE", "PIC_GNOME", "PIC_NONE",
/* 28*/  "PIC_NONE", "PIC_MAGE", "PIC_FURY", "PIC_NONE",
/* 32*/  "PIC_L32", "PIC_MALEWARRIOR", "PIC_NONE", "PIC_NONE",
/* 36*/  "PIC_NONE", "PIC_NONE", "PIC_L38", "PIC_UNBELIEVER",
/* 40*/  "PIC_NONE", "PIC_NONE", "PIC_NONE", "PIC_WHITEWIZARD",
/* 44*/  "PIC_NONE", "PIC_NONE", "PIC_OAKTREE", "PIC_REVIEW",
/* 48*/  "PIC_L48", "PIC_TEMPLE", "PIC_DESTBLDG", "PIC_L51",
/* 52*/  "PIC_L52", "PIC_L53", "PIC_L54", "PIC_L55",
/* 56*/  "PIC_LAKEBLDG", "PIC_GAMEOVER", "PIC_HAWKSSKEL", "PIC_CYANIS",
/* 60*/  "PIC_NONE", "PIC_L61", "PIC_NONE", "PIC_BEAR",
/* 64*/  "PIC_NONE", "PIC_NONE", "PIC_NONE", "PIC_NONE",
/* 68*/  "PIC_NONE", "PIC_BUILDING", "PIC_YETI", "PIC_MAGICMOUTH",
/* 72*/  "PIC_NONE", "PIC_NONE", "PIC_DEADMAN", "PIC_NONE",
/* 76*/  "PIC_NONE", "PIC_NONE", "PIC_L78", "PIC_L79",
/* 80*/  "PIC_NONE", "PIC_NONE", "PIC_NONE", "PIC_TAVERN",
/* 84*/  "PIC_NONE", "PIC_NONE", "PIC_NONE", "PIC_NONE",
/* 88*/  "PIC_NONE", "PIC_NONE", "PIC_NONE", "PIC_SERPENT",
/* 92*/  "PIC_NONE", "PIC_CREEPER", "PIC_NONE", "PIC_PYTHON",
/* 96*/  "PIC_NONE", "PIC_ZOMBIE", "PIC_FILCHER", "PIC_NONE",
/*100*/  "PIC_NONE", "PIC_NONE", "PIC_SHARK", "PIC_KRAKEN",
/*104*/  "PIC_NONE", "PIC_NONE", "PIC_NONE", "PIC_NONE",
/*108*/  "PIC_NONE", "PIC_HOWLER", "PIC_ROCKDEMON", "PIC_PITDEMON",
/*112*/  "PIC_NONE", "PIC_BLACKWIZARD", "PIC_NONE", "PIC_NONE",
/*116*/  "PIC_SCEADU", "PIC_NONE", "PIC_NONE", "PIC_WERRA",
/*120*/  "PIC_H13", "PIC_NONE", "PIC_URMECH", "PIC_CLANKER",
/*124*/  "PIC_REDBEARD", "PIC_WARBOT", "PIC_MALEMAGE", "PIC_NONE",
/*128*/  "PIC_SHADEWARRIOR", "PIC_SLAVE", "PIC_SKEPTIC", "PIC_NONE",
/*132*/  "PIC_PYRO", "PIC_FROSTMAGE", "PIC_NONE", "PIC_NONE",
/*136*/  "PIC_FEMALEWARRIOR", "PIC_NONE", "PIC_NONE", "PIC_GIANT",
/*140*/  "PIC_H26", "PIC_H27", "PIC_MALEMAGE", "PIC_PRIESTESS",
/*144*/  "PIC_NONE", "PIC_NONE", "PIC_NONE", "PIC_NONE",
/*148*/  "PIC_OCTOMECH", "PIC_DRAGON", "PIC_FOGGER", "PIC_NONE",
/*152*/  "PIC_RUSKI", "PIC_BLACKSLAYER", "PIC_VORTEX", "PIC_BLACKDRAGON",
/*156*/  "PIC_MONGOL", "PIC_NONE", "PIC_NONE", "PIC_NONE",
/*160*/  "PIC_NINJA", "PIC_NAZI", "PIC_NONE", "PIC_ROMAN",
/*164*/  "PIC_TITAN", "PIC_MERRYMAN", "PIC_REDEYE", "PIC_FEMALEMAGE",
/*168*/  "PIC_VAMPIRE", "PIC_NONE", "PIC_TARJAN", "PIC_NONE",
/*172*/  "PIC_SHAOLIN", "PIC_SCAVENGER", "PIC_NONE", "PIC_BIKER"
};


void indexToPic(uint8_t index, pic_t *p)
{
	if (picTran[index + 88] == 0xff) {
		p->hiflag = 0;
		p->index = index;
	} else {
		p->hiflag = 1;
		p->index = index + 88;
	}
}

uint8_t getPicIndex(pic_t p)
{
	return picTran[p.index];
}

/*
 * getPicMacro()
 */
btstring_t *getPicMacro(pic_t *p)
{
	return bts_strcpy(picMac[p->index]);
}

int doit()
{
	int i;

	for (i = 0; i < 88; i++) {
		if (picTran[i + 88] != 0xff) {
			 printf("{1, %d}\n", i + 88);
		} else if (picTran[i] != 0xff) {
			 printf("{0, %d}\n", i);
		}
	}
}
