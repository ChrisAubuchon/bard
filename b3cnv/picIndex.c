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
/* 88*/ 0xFF, 0xFF, 0xFF, 0,
/* 92*/ 0xFF, 1, 0xFF, 2,
/* 96*/ 0xFF, 3, 4, 0xFF,
/*100*/ 0xFF, 0xFF, 5, 6,
/*104*/ 0xFF, 0xFF, 0xFF, 0xFF,
/*108*/ 0xFF, 7, 8, 9,
/*112*/ 0xFF, 0x0A, 0xFF, 0xFF,
/*116*/ 0x0B, 0xFF, 0xFF, 0x0C,
/*120*/ 0x0D, 0xFF, 0x0E, 0x0F,
/*124*/ 0x10, 0x11, 0x12, 0xFF,
/*128*/ 0x13, 0x14, 0x15, 0xFF,
/*132*/ 0x16, 0x17, 0xFF, 0xFF,
/*136*/ 0x18, 0xFF, 0xFF, 0x19,
/*140*/ 0x1A, 0x1B, 0x1C, 0x1D,
/*144*/ 0xFF, 0xFF, 0xFF, 0xFF,
/*148*/ 0x1E, 0x1F, 0x20, 0xFF,
/*152*/ 0x21, 0x22, 0x23, 0x24,
/*156*/ 0x25, 0xFF, 0xFF, 0xFF,
/*160*/ 0x26, 0x27, 0xFF, 0x28,
/*164*/ 0x29, 0x2A, 0x2B, 0x2C,
/*168*/ 0x2D, 0xFF, 0x2E, 0xFF,
/*172*/ 0x2F, 0x30, 0xFF, 0x31
};

static uint8_t *picMac[] = {
/*  0*/  "PIC_CAMP", "PIC_VIPER", "PIC_HOOKFANG","PIC_SERPENT",
/*  4*/  "PIC_HOUND", "PIC_BLOODFIEND", "PIC_IMP", "PIC_EEL",
/*  8*/  "PIC_NEWT", "PIC_ZOMBIE", "PIC_GUARD", "PIC_NEFAST",
/* 12*/  "PIC_GORGON", "PIC_PIRANHA", "PIC_SHARK", "PIC_OCTOPUS",
/* 16*/  "PIC_PHANTOM", "PIC_BRILHASTI", "PIC_CORRUPTER", "PIC_PUMA",
/* 20*/  "PIC_ZEPHYR", "PIC_HERB", "PIC_NONE", "PIC_NONE",
/* 24*/  "PIC_RAINBOWDRAGON", "PIC_NONE", "PIC_GNOME", "PIC_NONE",
/* 28*/  "PIC_NONE", "PIC_MAGE", "PIC_FURY", "PIC_NONE",
/* 32*/  "PIC_KINGCOURT", "PIC_MALEWARRIOR", "PIC_NONE", "PIC_NONE",
/* 36*/  "PIC_NONE", "PIC_NONE", "PIC_REDMAGE", "PIC_UNBELIEVER",
/* 40*/  "PIC_NONE", "PIC_NONE", "PIC_NONE", "PIC_WHITEWIZARD",
/* 44*/  "PIC_NONE", "PIC_NONE", "PIC_OAKTREE", "PIC_REVIEW",
/* 48*/  "PIC_FEMALEWARRIOR", "PIC_TEMPLE", "PIC_DESTBLDG", "PIC_OGRE",
/* 52*/  "PIC_TREASURE", "PIC_CHEST", "PIC_MALEMAGE", "PIC_WITCH",
/* 56*/  "PIC_LAKEBLDG", "PIC_GAMEOVER", "PIC_HAWKSSKEL", "PIC_CYANIS",
/* 60*/  "PIC_NONE", "PIC_DRAGON", "PIC_NONE", "PIC_BEAR",
/* 64*/  "PIC_NONE", "PIC_NONE", "PIC_NONE", "PIC_NONE",
/* 68*/  "PIC_NONE", "PIC_BUILDING", "PIC_YETI", "PIC_MAGICMOUTH",
/* 72*/  "PIC_NONE", "PIC_NONE", "PIC_DEADMAN", "PIC_NONE",
/* 76*/  "PIC_NONE", "PIC_NONE", "PIC_EYE", "PIC_FEMALEMAGE",
/* 80*/  "PIC_NONE", "PIC_NONE", "PIC_NONE", "PIC_TAVERN",
/* 84*/  "PIC_NONE", "PIC_NONE", "PIC_NONE", "PIC_NONE",
/* 88*/  "PIC_NONE", "PIC_NONE", "PIC_NONE", "PIC_H0",
/* 92*/  "PIC_NONE", "PIC_H1", "PIC_NONE", "PIC_H2",
/* 96*/  "PIC_NONE", "PIC_H3", "PIC_H4", "PIC_NONE",
/*100*/  "PIC_NONE", "PIC_NONE", "PIC_H5", "PIC_H6",
/*104*/  "PIC_NONE", "PIC_NONE", "PIC_NONE", "PIC_NONE",
/*108*/  "PIC_NONE", "PIC_H7", "PIC_ROCKDEMON", "PIC_PITDEMON",
/*112*/  "PIC_NONE", "PIC_BLACKWIZARD", "PIC_NONE", "PIC_NONE",
/*116*/  "PIC_SCEADU", "PIC_NONE", "PIC_NONE", "PIC_WERRA",
/*120*/  "PIC_H13", "PIC_NONE", "PIC_URMECH", "PIC_CLANKER",
/*124*/  "PIC_REDBEARD", "PIC_WARBOT", "PIC_H18", "PIC_NONE",
/*128*/  "PIC_SHADEWARRIOR", "PIC_SLAVE", "PIC_SKEPTIC", "PIC_NONE",
/*132*/  "PIC_PYRO", "PIC_FROSTMAGE", "PIC_NONE", "PIC_NONE",
/*136*/  "PIC_H24", "PIC_NONE", "PIC_NONE", "PIC_H25",
/*140*/  "PIC_H26", "PIC_H27", "PIC_H28", "PIC_H29",
/*144*/  "PIC_NONE", "PIC_NONE", "PIC_NONE", "PIC_NONE",
/*148*/  "PIC_OCTOMECH", "PIC_H31", "PIC_FOGGER", "PIC_NONE",
/*152*/  "PIC_RUSKI", "PIC_BLACKSLAYER", "PIC_VORTEX", "PIC_BLACKDRAGON",
/*156*/  "PIC_MONGOL", "PIC_NONE", "PIC_NONE", "PIC_NONE",
/*160*/  "PIC_NINJA", "PIC_NAZI", "PIC_NONE", "PIC_ROMAN",
/*164*/  "PIC_TITAN", "PIC_MERRYMAN", "PIC_HI43", "PIC_HI44",
/*168*/  "PIC_VAMPIRE", "PIC_NONE", "PIC_TARJAN", "PIC_NONE",
/*172*/  "PIC_SHAOLIN", "PIC_SCAVENGER", "PIC_NONE", "PIC_BIKER"
};

static uint8_t *picMacList[] = {
  "PIC_NONE",
  "PIC_CAMP", "PIC_VIPER", "PIC_HOOKFANG","PIC_SERPENT",
  "PIC_HOUND", "PIC_BLOODFIEND", "PIC_IMP", "PIC_EEL",
  "PIC_NEWT", "PIC_ZOMBIE", "PIC_GUARD", "PIC_NEFAST",
  "PIC_GORGON", "PIC_PIRANHA", "PIC_SHARK", "PIC_OCTOPUS",
  "PIC_PHANTOM", "PIC_BRILHASTI", "PIC_CORRUPTER", "PIC_PUMA",
  "PIC_ZEPHYR", "PIC_HERB",  
  "PIC_RAINBOWDRAGON",  "PIC_GNOME", 
   "PIC_MAGE", "PIC_FURY", 
  "PIC_KINGCOURT", "PIC_MALEWARRIOR",  
    "PIC_REDMAGE", "PIC_UNBELIEVER",
     "PIC_WHITEWIZARD",
    "PIC_OAKTREE", "PIC_REVIEW",
  "PIC_FEMALEWARRIOR", "PIC_TEMPLE", "PIC_DESTBLDG", "PIC_OGRE",
  "PIC_TREASURE", "PIC_CHEST", "PIC_MALEMAGE", "PIC_WITCH",
  "PIC_LAKEBLDG", "PIC_GAMEOVER", "PIC_HAWKSSKEL", "PIC_CYANIS",
   "PIC_DRAGON",  "PIC_BEAR",
     
   "PIC_BUILDING", "PIC_YETI", "PIC_MAGICMOUTH",
    "PIC_DEADMAN", 
    "PIC_EYE", "PIC_FEMALEMAGE",
     "PIC_TAVERN",
     
  "PIC_ROCKDEMON", "PIC_PITDEMON",
   "PIC_BLACKWIZARD",  
  "PIC_SCEADU",   "PIC_WERRA",
  "PIC_URMECH", "PIC_CLANKER",
  "PIC_REDBEARD", "PIC_WARBOT",
  "PIC_SHADEWARRIOR", "PIC_SLAVE", "PIC_SKEPTIC", 
  "PIC_PYRO", "PIC_FROSTMAGE",  
  "PIC_OCTOMECH", "PIC_FOGGER", 
  "PIC_RUSKI", "PIC_BLACKSLAYER", "PIC_VORTEX", "PIC_BLACKDRAGON",
  "PIC_MONGOL",   
  "PIC_NINJA", "PIC_NAZI",  "PIC_ROMAN",
  "PIC_TITAN", "PIC_MERRYMAN",
  "PIC_VAMPIRE",  "PIC_TARJAN", 
  "PIC_SHAOLIN", "PIC_SCAVENGER",  "PIC_BIKER", 0
};

void printPicMacro(uint8_t pi)
{
  uint8_t index = 0xff;
  uint8_t hi = 0;

  if (picTran[pi] == 0xff) {
    index = pi + 88;
    hi = 1;
  } else {
    index = pi;
  }

  printf("%s", picMac[index]);
}

btstring_t *getPicMacro(pic_t *p)
{
	uint8_t		index;

	if (p->hiflag)
		index = p->index + 88;
	else
		index = p->index;

	return bts_strcpy(picMac[index]);
}

void printPicMacros(void)
{
  uint32_t i = 0;

  while (picMacList[i] != 0) {
    printf("#define %s		%d\n", picMacList[i], i);
    i++;
  }
}

uint8_t *getHiLoPicMacro(uint8_t hiflag, uint8_t index)
{
  uint32_t i;

  for (i = 0; i < 88; i++) {
    if (hiflag) {
      if (picTran[i + 88] == index)
	return picMac[i+88];
    } else {
      if (picTran[i] == index)
	return picMac[i];
    }
  }

  return NULL;
}

