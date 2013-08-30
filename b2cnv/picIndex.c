#include <b2lib.h>
#include <fileio.h>

static uint8_t *picMac[] = {
/*  0*/"PIC_WARRIOR", "PIC_PALADIN", "PIC_ROGUE", "PIC_BARD",
/*  4*/"PIC_HUNTER", "PIC_MONK", "PIC_CONJURER", "PIC_MAGICIAN",
/*  8*/"PIC_SORCEROR", "PIC_WIZARD", "PIC_ARCHMAGE", "PIC_TREASURE",
/* 12*/"PIC_GAMEOVER", "PIC_MAGICMOUTH", "PIC_BANKINT", "PIC_IRONGATE",
/* 16*/"PIC_TEMPLEEXT", "PIC_BAREXT", "PIC_GARTHEXT", "PIC_GUILDINT",
/* 20*/"PIC_CREDITS", "PIC_GUILDEXT", "PIC_EMPTYHUT", "PIC_CHEST",
/* 24*/"PIC_BARINT", "PIC_NONE1", "PIC_GARTHINT", "PIC_TEMPLEINT",
/* 28*/"PIC_REVINT", "PIC_ROSCOEINT", "PIC_NONE2", "PIC_EMPTYBLDG",
/* 32*/"PIC_DRAGON", "PIC_MAGE", "PIC_ZOMBIE", "PIC_DAEMON",
/* 36*/"PIC_GIANT", "PIC_SKELETON", "PIC_ARCHER", "PIC_GUARDIAN",
/* 40*/"PIC_VAMPIRE", "PIC_MEDUSA", "PIC_KOBOLD", "PIC_NINJA",
/* 44*/"PIC_NONE3", "PIC_ZENDAR", "PIC_TOXICGIANT", "PIC_GOLEM",
/* 48*/"PIC_LIZARD", "PIC_VICTORY", "PIC_FORGESEGMENT", "PIC_NONE4",
/* 52*/"PIC_ROCKGIANT", "PIC_PRIEST", "PIC_FIEND", "PIC_OGRE",
/* 56*/"PIC_CLOUDGIANT", "PIC_DREN", "PIC_BUGGER", "PIC_GUARD",
/* 60*/"PIC_SAGE", "PIC_DEATHDEALER", "PIC_DOG", "PIC_TITAN",
};

static uint8_t *picMacList[] = {
/*  0*/ "PIC_NONE","PIC_WARRIOR","PIC_PALADIN","PIC_ROGUE",
/*  4*/ "PIC_BARD","PIC_HUNTER","PIC_MONK","PIC_CONJURER",
/*  8*/ "PIC_MAGICIAN","PIC_SORCEROR","PIC_WIZARD","PIC_ARCHMAGE",
/* 12*/ "PIC_TREASURE","PIC_GAMEOVER","PIC_MAGICMOUTH","PIC_BANKINT",
/* 16*/ "PIC_IRONGATE","PIC_TEMPLEEXT","PIC_BAREXT","PIC_GARTHEXT",
/* 20*/ "PIC_GUILDINT","PIC_CREDITS","PIC_GUILDEXT","PIC_EMPTYHUT",
/* 24*/ "PIC_CHEST","PIC_BARINT","PIC_GARTHINT","PIC_TEMPLEINT",
/* 28*/ "PIC_REVINT","PIC_ROSCOEINT","PIC_EMPTYBLDG","PIC_DRAGON",
/* 32*/ "PIC_MAGE","PIC_ZOMBIE","PIC_DAEMON","PIC_GIANT",
/* 36*/ "PIC_SKELETON","PIC_ARCHER","PIC_GUARDIAN","PIC_VAMPIRE",
/* 40*/ "PIC_MEDUSA","PIC_KOBOLD","PIC_NINJA","PIC_ZENDAR",
/* 44*/ "PIC_TOXICGIANT","PIC_GOLEM","PIC_LIZARD","PIC_VICTORY",
/* 48*/ "PIC_FORGESEGMENT","PIC_ROCKGIANT","PIC_PRIEST","PIC_FIEND",
/* 52*/ "PIC_OGRE","PIC_CLOUDGIANT","PIC_DREN","PIC_BUGGER",
/* 56*/ "PIC_GUARD","PIC_SAGE","PIC_DEATHDEALER","PIC_DOG",
/* 60*/ "PIC_TITAN",
};

void printPicMacro(uint8_t pi)
{
  printf("%s", picMac[pi]);
}

void printPicMacros(void)
{
  uint32_t i;

  for (i = 0; i < 61; i++) 
    printf("#define %s\t\t%d\n", picMacList[i], i );
}

btstring_t *getPicMacro(uint8_t pi)
{
  return bts_strcpy(picMac[pi]);
}

/*
void outputPicXML(void)
{
  uint8_t fname[32];
  uint32_t i;
  uint8_t *gfx;

  printf("<b2_pics>\n");
  for (i = 0; i < 63; i++) {
    printf("  <%s>\n", picMac[i]);
    sprintf(fname, "bigpic-%d.b64", i);
    gfx = readFile(fname);
    printf("    <gfx>\n");
    printf(gfx);
    printf("    </gfx>\n");
    printf("  </%s>\n", picMac[i]);
    free(gfx);
  }

  printf("</b2_pics>\n");
}
*/
