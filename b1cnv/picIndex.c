#include <b1lib.h>
#include <fileio.h>

static uint8_t *picMac[] = {
/* 0*/"PIC_WARRIOR", "PIC_PALADIN", "PIC_ROGUE", "PIC_BARD",
/* 4*/"PIC_HUNTER", "PIC_MONK", "PIC_CONJURER", "PIC_MAGICIAN",
/* 8*/"PIC_SORCEROR", "PIC_WIZARD", "PIC_TREASURE", "PIC_GAMEOVER",
/*12*/"PIC_MAGICMOUTH", "PIC_BALROG", "PIC_OGRE", "PIC_HOBBIT",
/*16*/"PIC_MERCENARY", "PIC_WMAGE", "PIC_ZOMBIE", "PIC_WRAITH",
/*20*/"PIC_DRAGON", "PIC_GIANT", "PIC_SKELETON", "PIC_THIEF",
/*24*/"PIC_HOUND", "PIC_FRED", "PIC_SPIDER", "PIC_GOBLIN",
/*28*/"PIC_SEEKER", "PIC_SMAGE", "PIC_JADEMONK", "PIC_SAMURAI",
/*32*/"PIC_HYDRA", "PIC_LURKER", "PIC_STATUE", "PIC_CASTLEEXT",
/*36*/"PIC_IRONGATE", "PIC_CITYSTATUE", "PIC_CITYGATE", "PIC_VAMPIRE",
/*40*/"PIC_SPECTRE", "PIC_DEMON", "PIC_TEMPLEEXT", "PIC_BAREXT",
/*44*/"PIC_MANGAR", "PIC_GARTHEXT", "PIC_MADGOD", "PIC_GUILDINT",
/*48*/"PIC_KOBOLD", "PIC_NOMAD", "PIC_WIGHT", "PIC_CREDITS",
/*52*/"PIC_GUILDEXT", "PIC_KYLEARAN", "PIC_CHEST", "PIC_BARINT",
/*56*/"PIC_GARTHINT", "PIC_TEMPLEINT", "PIC_REVINT", "PIC_ROSCOEINT",
/*60*/"PIC_TOWERINT", "PIC_HARKYNCASTLE", "PIC_EMPTYBLDG"
};

btstring_t *getPicMacro(uint8_t pi)
{
  return bts_strcpy(picMac[pi]);
}

