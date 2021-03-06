#include <b1lib.h>

uint8_t *monStr[] = {
  "Kobold^^s^",
  "Hobbit^^s^",
  "Gnome^^s^",
  "Dwar^f^ves^",
  "Thie^f^ves^",
  "Hobgoblin^^s^",
  "Conjurer^^s^",
  "Magician^^s^",
  "Orc^^s^",
  "Skeleton^^s^",
  "Nomad^^s^",
  "Spider^^s^",
  "Mad Dog^^s^",
  "Barbarian^^s^",
  "Mercenar^y^ies^",
  "Wol^f^ves^",
  "Jade Monk^^s^",
  "Half Orc^^s^",
  "Swordsm^an^en^",
  "Zombie^^s^",
  "Conjurer^^s^",
  "Magician^^s^",
  "Sorcerer^^s^",
  "Wizard^^s^",
  "Samurai",
  "Black Widow^^s^",
  "Assassin^^s^",
  "Werewol^f^ves^",
  "Ogre^^s^",
  "Wight^^s^",
  "Statue^^s^",
  "Bladesm^an^en^",
  "Goblin Lord^^s^",
  "Master Thie^f^ves^",
  "Conjurer^^s^",
  "Magician^^s^",
  "Sorcerer^^s^",
  "Wizard^^s^",
  "Ninja^^s^",
  "Spinner^^s^",
  "Scarlet Monk^^s^",
  "Doppleganger^^s^",
  "Stone Giant^^s^",
  "Ogre Magician^^s^",
  "Jackalwere^^s^",
  "Stone Elemental^^s^",
  "Blue Dragon^^s^",
  "Seeker^^s^",
  "Dwarf King^^s^",
  "Samurai Lord^^s^",
  "Ghoul^^s^",
  "Conjurer^^s^",
  "Magician^^s^",
  "Sorcerer^^s^",
  "Wizard^^s^",
  "Azure Monk^^s^",
  "Weretiger^^s^",
  "Hydra^^s^",
  "Green Dragon^^s^",
  "Wraith^^s^",
  "Lurker^^s^",
  "Fire Giant^^s^",
  "Copper Dragon^^s^",
  "Ivory Monk^^s^",
  "Shadow^^s^",
  "Berserker^^s^",
  "Conjurer^^s^",
  "Magician^^s^",
  "Sorcerer^^s^",
  "Wizard^^s^",
  "White Dragon^^s^",
  "Ice Giant^^s^",
  "Eye Sp^y^ies^",
  "Ogre Lord^^s^",
  "Body Snatcher^^s^",
  "Xorn^^s^",
  "Phantom^^s^",
  "Lesser Demon^^s^",
  "Fred^^s^",
  "Conjurer^^s^",
  "Magician^^s^",
  "Sorcerer^^s^",
  "Wizard^^s^",
  "Master Ninja^^s^",
  "War Giant^^s^",
  "Warrior Elite^^s^",
  "Bone Crusher^^s^",
  "Ghost^^s^",
  "Grey Dragon^^s^",
  "Basilisk^^s^",
  "Evil Eye^^s^",
  "Mimic^^s^",
  "Golem^^s^",
  "Vampire^^s^",
  "Demon^^s^",
  "Bandersnatch^^es^",
  "Maze Dweller^^s^",
  "Mongo^^s^",
  "Mangar Guard^^s^",
  "Gimp^^s^",
  "Red Dragon^^s^",
  "Titan^^s^",
  "Master Conjurer^^s^",
  "Master Magician^^s^",
  "Master Sorcerer^^s^",
  "Mind Shadow^^s^",
  "Spectre^^s^",
  "Cloud Giant^^s^",
  "Beholder^^s^",
  "Vampire Lord^^s^",
  "Greater Demon^^s^",
  "Master Wizard^^s^",
  "Mad God^^s^",
  "Maze Master^^s^",
  "Death Denizen^^s^",
  "Jabberwock^^s^",
  "Black Dragon^^s^",
  "Mangar",
  "Crystal Golem^^s^",
  "Soul Sucker^^s^",
  "Storm Giant^^s^",
  "Ancient Enem^y^ies^",
  "Balrog^^s^",
  "Lich^^es^",
  "Archmage^^s^",
  "Demon Lord^^s^",
  "Old M^an^en^"
};

uint16_t diceMask[] = {
  2, 4, 8, 16, 32, 64, 128, 256
};

uint8_t monAttrib[] = {
  0x40,0x60,0x21,0x61,0x22,0x42,0x21,0x21,0x63,0x84,0x84,0x44,0x43,
  0xA4,0x85,0xA4,0x23,0xC5,0xC6,0xC5,0x23,0x23,0x23,0x23,0x46,
  0x25,0x26,0x45,0x86,0x66,0x86,0xC6,0xC7,0xA5,0x25,0x25,0x25,
  0x25,0x86,0xC6,0x46,0x87,0x27,0xE7,0xE8,0x28,0x69,0x28,0x48,
  0x69,0x29,0x27,0x27,0x27,0x67,0x88,0x89,0x49,0x89,0xAA,0x49,0xCA,
  0xEA,0x4A,0x6B,0xAA,0xA9,0xA9,0xC9,0xA9,0xEB,0x2B,0x2A,
  0x2A,0x6A,0x4B,0x4B,0xAC,0x4A,0x2B,0x2B,0x2B,0x4B,0x6C,0x6D,0x4D,
  0x4C,0x6C,0x8C,0x4B,0x2B,0x4C,0x4D,0x8D,0x2E,0xCE,0xAF,0xEF,
  0x50,0x2F,0x50,0x70,0xED,0xED,0xED,0xCF,0x50,0x71,0x30,0x72,
  0xD3,0x2F,0x70,0xB2,0xB0,0x91,0xB3,0xF4,0xD3,0xB2,0xF4,
  0xF3,0xD3,0xF4,0x54,0xF4,0x14
};

uint8_t monMask[] = {
  0x12,0x12,0x22,0x22,0x21,0x22,0x21,0x21,0x22,0x21,0x21,0x21,0x22,
  0x22,0x21,0x22,0x21,0x22,0x21,0x21,0x21,0x21,0x21,0x21,0x32,0x32,
  0x32,0x31,0x31,0x31,0x31,0x32,0x31,0x31,0x32,0x32,0x32,0x31,0x32,
  0x32,0x31,0x30,0x42,0x32,0x32,0x41,0x42,0x42,0x42,0x41,0x42,0x42,
  0x42,0x42,0x41,0x41,0x42,0x42,0x41,0x42,0x42,0x42,0x41,0x42,0x42,
  0x42,0x42,0x42,0x41,0x42,0x41,0x52,0x53,0x52,0x53,0x52,0x52,0x52,
  0x50,0x52,0x52,0x52,0x51,0x52,0x52,0x53,0x52,0x50,0x51,0x52,0x53,
  0x50,0x52,0x52,0x62,0x54,0x15,0x51,0x64,0x66,0x61,0x62,0x52,0x52,
  0x52,0x53,0x62,0x62,0x61,0x61,0x62,0x61,0x63,0x64,0x15,0x62,0x62,
  0x70,0x60,0x62,0x62,0x62,0x66,0x72,0x72,0x72,0x71
};

uint8_t monPicIndex[] = {
  0x30,0x0F,0x30,0x0F,0x17,0x1B,0x11,0x11,0x1B,0x16,0x31,0x1A,0x18,
  0x31,0x10,0x18,0x1E,0x1B,0x10,0x12,0x11,0x11,0x1D,0x1D,0x1F,0x1A,
  0x17,0x18,0x0E,0x32,0x22,0x10,0x30,0x17,0x11,0x11,0x1D,0x1D,0x17,
  0x1A,0x1E,0x12,0x15,0x0E,0x18,0x22,0x14,0x1C,0x0F,0x1F,0x32,0x11,
  0x11,0x1D,0x1D,0x1E,0x18,0x20,0x14,0x13,0x21,0x15,0x14,0x1E,0x13,
  0x31,0x11,0x11,0x1D,0x1D,0x14,0x15,0x1C,0x0E,0x12,0x22,0x13,0x13,
  0x19,0x11,0x11,0x1D,0x1D,0x17,0x15,0x10,0x31,0x21,0x14,0x20,0x1C,
  0x12,0x22,0x27,0x13,0x18,0x21,0x19,0x1F,0x12,0x14,0x15,0x11,0x11,
  0x1D,0x21,0x28,0x15,0x1C,0x27,0x13,0x1D,0x2E,0x10,0x21,0x20,0x14,
  0x2C,0x22,0x1C,0x15,0x13,0x0D,0x28,0x11,0x29,0x19
};

uint8_t monPri[] = {
  0,0,1,1,1,1,0,0,2,2,2,2,1,3,2,3,4,3,3,4,2,2,2,2,4,
  0x24,3,4,5,0x84,4,5,5,4,4,4,4,4,6,0x26,7,6,8,7,7,7,8,7,
  7,8,0x88,6,6,6,6,0x0A,9,9,0x0A,0x6A,0x0A,0x0B,0x0B,0x0D,0x6B,
  0x0B,8,8,8,8,0x0B,0x0C,0x2B,0x0C,0xAB,0x0C,0x4D,0x8E,0x0C,0x0A,
  0x0A,0x0A,0x0A,0xEE,0x0F,0x0F,0x0E,0x90,0x11,0xD0,0x30,0x10,0x12,
  0x53,0x74,0x13,0x13,0x14,0x14,0x10,0x17,0x16,0x10,0x10,0x10,0x13,
  0x56,0x18,0x36,0x59,0xBB,0x12,0x15,0x15,0x15,0x18,0x1A,0xF8,0x1C,
  0x1C,0x1E,0x9D,0x75,0xB7,0x15,0xDF,0xFF
};

uint8_t monReward[] = {
  0x3C,0x46,0x50,0x50,0x5A,0x5A,0x64,
  0x64,0x64,0x6E,0x78,0x96,0xB4,0xC8,0xDC,
  1,1,1,2,2,2,2,3,
  3,4,4,4,4,4,4,4,
  5,5,5,5,6,6,6,6,
  7,7,7,7,8,8,8,8,
  9,9,9,9,9,9,9,9,
  0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,0x0B,
  0x0B,0x0B,0x0B,0x0B,0x0C,0x0C,0x0C,0x0C,
  0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,
  0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,0x0D,
  0x0D,0x0D,0x0D,0x0D,0x0E,0x0E,0x0E,0x0E,
  0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,
  0x0E,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x0F
};

uint8_t monAttacks[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,1,1,2,0,0,0x19,0x13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,1,2,3,0,0x19,0x13,0x1B,0,0x0B,0x0B,0x0C,
  0,1,0x19,0x22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,3,4,0,0x1B,
  0x1C,0x1D,0,0x0B,0x0C,0x0D,0,4,0x1B,0x22,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0xFE,0xFE,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0x57,0x57,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,5,
  0,0x1B,0x1D,0x1E,0,0x0E,0x10,0x11,0,4,0x1D,0x23,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0x58,0x58,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0x59,0x59,0,0,0,0,0,0,0,0,0,0,0,0,0,5,6,6,0,0x1B,0x1E,0x1F,
  0,6,0x1D,0x23,0,0x10,0x11,0x12,0,0,0x4F,0x4F,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x11,0,0,0,0x14,
  0,9,8,7,0,0x1D,0x1C,0x1F,0,0x12,0x15,0x16,0,0x25,0x24,0x1F,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x50,0x50,0,
  0,0,0,0,0,0x1D,0x0E,0,0,0xFE,0xFE,0,0,0,0,0,0,0,0x0D,0,
  0,0,0x50,0,0,0,0,0,0,0,0,0,0,0x20,0x20,0,0,0,0,0,0,0,0,
  0,0,0x51,0x51,0,0,0,0,0,0x0A,9,9,0,0x1F,0x20,0x21,0,0x15,0x16,
  0x18,0,0,0,0,0,0,0,0,0,0,0,0,0,0x1E,0x1F,0x21,0,0,0x24,0x21,
  0,0,0x25,0x52,0,0x25,0x26,0x26,0,0,0x20,7,0,0,0,0x15,0,0,0,
  0,0,0,0x53,0x53,0,0,0x54,0x54,0x26,0x18,0x21,0x1F,0,0,0,0,0,
  0,0x55,0x55,0,0,0,0,0,0,0,0,0,0,0,0x21,0,0x26,0x21,0x0D,0x26,
  0x18,0x21,0x1F,0,0,0x26,0x56,0,0,0x15,0x15
};

uint8_t monPronoun[] = {
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0xFF, 1, 1, 1,
  1, 1, 0, 0, 0, 0, 0, 1,
  0, 1, 0, 0, 0, 1, 1, 0,
  0, 0, 0,0xFF,0xFF, 0, 0, 0,
  0, 0, 0, 0, 1, 0, 1, 0,
  0, 1, 0, 0
};

uint8_t *monLevel[] = {
/*0*/  "{0}", "{0}", "{0}", "{0}", "{0}", "{0}", "{0}", "{0}", 
/*8*/  "{0,1,2}", "{0,1,2}", "{0,1,2}", "{0,1,2}", 
/*12*/ "{0,1,2}", "{0,1,2}", "{0,1,2}", "{0,1,2}", 
/*16*/ "{1,2,3}", "{1,2,3}", "{1,2,3}", "{1,2,3}", 
/*20*/ "{1,2,3}", "{1,2,3}", "{1,2,3}", "{1,2,3}", 
/*24*/ "{2,3}", "{2,3}", "{2,3}", "{2,3}", "{2,3}", "{2,3}", "{2,3}", "{2,3}", 
/*32*/ "{2,3,4}", "{2,3,4}", "{2,3,4}", "{2,3,4}", 
/*36*/ "{2,3,4}", "{2,3,4}", "{2,3,4}", "{2,3,4}", 
/*40*/ "{3,4}", "{3,4}", "{3,4}", "{3,4}", "{3,4}", "{3,4}", "{3,4}", "{3,4}", 
/*48*/ "{4,5}", "{4,5}", "{4,5}", "{4,5}", "{4,5}", "{4,5}", "{4,5}", "{4,5}", 
/*56*/ "{4,5}", "{4,5}", "{4,5}", "{4,5}", "{4,5}", "{4,5}", "{4,5}", "{4,5}", 
/*64*/ "{5,6}", "{5,6}", "{5,6}", "{5,6}", "{5,6}", "{5,6}", "{5,6}", "{5,6}", 
/*72*/ "{5,6}", "{5,6}", "{5,6}", "{5,6}", "{5,6}", "{5,6}", "{5,6}", "{5,6}", 
/*80*/ "{6}", "{6,7}", "{6,7}", "{6,7}", "{6,7}", "{6,7}", "{6,7}", "{6,7}", 
/*88*/ "{6,7}", "{6,7}", "{6,7}", "{6,7}", "{6,7}", "{6,7}", "{6,7}", "{6,7}", 
/*96*/ "{7}", "{7}", "{7}", "{7}", "{7}", "{7}", "{7}", "{7}", 
/*104*/"{7}", "{7}", "{7}", "{7}", "{7}", "{7}", "{7}", "{7}", 
/*112*/"{7}", "{7}", "{7}", "{7}", "{7}", "{7}", "{7}", "{7}",
/*120*/"{-1}", "{-1}", "{-1}", "{-1}", "{-1}", "{-1}", "{-1}", "{-1}",
};

uint8_t *monMacro[] = {
/*  0*/ "M_KOBOLD", "M_HOBBIT", "M_GNOME", "M_DWARF",
/*  4*/ "M_THIEF", "M_HOBGOBLIN", "M_CONJURER_0", "M_MAGICIAN_0",
/*  8*/ "M_ORC", "M_SKELETON", "M_NOMAD", "M_SPIDER",
/* 12*/ "M_MAD_DOG", "M_BARBARIAN", "M_MERCENARY", "M_WOLF",
/* 16*/ "M_JADE_MONK", "M_HALF_ORC", "M_SWORDSMAN", "M_ZOMBIE",
/* 20*/ "M_CONJURER_1", "M_MAGICIAN_1", "M_SORCERER_1", "M_WIZARD_1",
/* 24*/ "M_SAMURAI", "M_BLACK_WIDOW", "M_ASSASSIN", "M_WEREWOLF",
/* 28*/ "M_OGRE", "M_WIGHT", "M_STATUE", "M_BLADESMAN",
/* 32*/ "M_GOBLIN_LORD", "M_MASTER_THIEF", "M_CONJURER_2", "M_MAGICIAN_2",
/* 36*/ "M_SORCERER_2", "M_WIZARD_2", "M_NINJA", "M_SPINNER",
/* 40*/ "M_SCARLET_MONK", "M_DOPPLEGANGER", "M_STONE_GIANT", "M_OGRE_MAGICIAN",
/* 44*/ "M_JACKALWERE", "M_STONE_ELEMENTAL", "M_BLUE_DRAGON", "M_SEEKER",
/* 48*/ "M_DWARF_KING", "M_SAMURAI_LORD", "M_GHOUL", "M_CONJURER_3",
/* 52*/ "M_MAGICIAN_3", "M_SORCERER_3", "M_WIZARD_3", "M_AZURE_MONK",
/* 56*/ "M_WERETIGER", "M_HYDRA", "M_GREEN_DRAGON", "M_WRAITH",
/* 60*/ "M_LURKER", "M_FIRE_GIANT", "M_COPPER_DRAGON", "M_IVORY_MONK",
/* 64*/ "M_SHADOW", "M_BERSERKER", "M_CONJURER", "M_MAGICIAN",
/* 68*/ "M_SORCERER", "M_WIZARD", "M_WHITE_DRAGON", "M_ICE_GIANT",
/* 72*/ "M_EYE_SPY", "M_OGRE_LORD", "M_BODY_SNATCHER", "M_XORN",
/* 76*/ "M_PHANTOM", "M_LESSER_DEMON", "M_FRED", "M_CONJURER_4",
/* 80*/ "M_MAGICIAN_4", "M_SORCERER_4", "M_WIZARD_4", "M_MASTER_NINJA",
/* 84*/ "M_WAR_GIANT", "M_WARRIOR_ELITE", "M_BONE_CRUSHER", "M_GHOST",
/* 88*/ "M_GREY_DRAGON", "M_BASILISK", "M_EVIL_EYE", "M_MIMIC",
/* 92*/ "M_GOLEM", "M_VAMPIRE", "M_DEMON", "M_BANDERSNATCH",
/* 96*/ "M_MAZE_DWELLER", "M_MONGO", "M_MANGAR_GUARD", "M_GIMP",
/*100*/ "M_RED_DRAGON", "M_TITAN", "M_MASTER_CONJURER", "M_MASTER_MAGICIAN",
/*104*/ "M_MASTER_SORCERER", "M_MIND_SHADOW", "M_SPECTRE", "M_CLOUD_GIANT",
/*108*/ "M_BEHOLDER", "M_VAMPIRE_LORD", "M_GREATER_DEMON", "M_MASTER_WIZARD",
/*112*/ "M_MAD_GOD", "M_MAZE_MASTER", "M_DEATH_DENIZEN", "M_JABBERWOCK",
/*116*/ "M_BLACK_DRAGON", "M_MANGAR", "M_CRYSTAL_GOLEM", "M_SOUL_SUCKER",
/*120*/ "M_STORM_GIANT", "M_ANCIENT_ENEMY", "M_BALROG", "M_LICH",
/*124*/ "M_ARCHMAGE", "M_DEMON_LORD", "M_OLD_MAN", "M_TARGET_DUMMY",
/*128*/ "M_JOE_THE_SWORD", "M_THOR"
};

uint8_t monAttType[] = {
  0,0,0,0,4,0,0,
  0,0,2,0,3,3,0,0,
  3,1,0,0,6,0,0,0,
  0,4,3,4,2,0,6,1,
  0,0,4,0,0,0,0,4,
  3,1,0,5,0,2,1,2,
  6,0,4,6,0,0,0,0,
  1,3,3,2,6,2,5,2,
  1,6,0,0,0,0,0,2,
  5,7,0,0,1,6,2,0,
  0,0,0,0,4,5,0,5,
  6,2,3,7,0,5,2,2,
  3,0,0,0,1,2,5,0,
  0,0,6,6,5,7,2,2,
  0,5,0,3,2,2,5,5,
  7,5,6,0,6,0,2,0,
};

uint8_t monSpellTrans[] = {
  0,1,4,6,8,0x0B,0x0C,0x0E,0x10,0x11,0x12,0x0B,0x17,0x19,0x38,0x1D,
  0x1E,0x21,0x23,0x24,0x25,0x11,0x28,0x29,0x2A,0x31,0x34,0x35,0x36,
  0x37,0x38,0x39,0x3D,0x41,0x42,0x44,0x46,0x48,0x4E,0
};
