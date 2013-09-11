#include <b2lib.h>
#include <spell.h>

uint8_t *spellClass[] = {
/*  0*/ "Conjurer", "Conjurer", "Conjurer", "Conjurer",
/*  4*/ "Conjurer", "Conjurer", "Conjurer", "Conjurer",
/*  8*/ "Conjurer", "Conjurer", "Conjurer", "Conjurer",
/* 12*/ "Conjurer", "Conjurer", "Conjurer", "Conjurer",
/* 16*/ "Conjurer", "Conjurer", "Conjurer", "Magician",
/* 20*/ "Magician", "Magician", "Magician", "Magician",
/* 24*/ "Magician", "Magician", "Magician", "Magician",
/* 28*/ "Magician", "Magician", "Magician", "Magician",
/* 32*/ "Magician", "Magician", "Magician", "Magician",
/* 36*/ "Magician", "Magician", "Sorcerer", "Sorcerer",
/* 40*/ "Sorcerer", "Sorcerer", "Sorcerer", "Sorcerer",
/* 44*/ "Sorcerer", "Sorcerer", "Sorcerer", "Sorcerer",
/* 48*/ "Sorcerer", "Sorcerer", "Sorcerer", "Sorcerer",
/* 52*/ "Sorcerer", "Sorcerer", "Sorcerer", "Sorcerer",
/* 56*/ "Sorcerer", "Wizard", "Wizard", "Wizard",
/* 60*/ "Wizard", "Wizard", "Wizard", "Wizard",
/* 64*/ "Wizard", "Wizard", "Wizard", "Wizard",
/* 68*/ "Wizard", "Wizard", "Wizard", "Archmage",
/* 72*/ "Archmage", "Archmage", "Archmage", "Archmage",
/* 76*/ "Archmage", "Archmage", "Archmage"
};

uint8_t spellLevel[] = {
/*  0*/ 1, 1, 1, 2,
/*  4*/ 2, 2, 3, 3,
/*  8*/ 3, 4, 4, 5,
/* 12*/ 5, 6, 6, 7,
/* 16*/ 7, 7, 7, 1,
/* 20*/ 1, 1, 2, 2,
/* 24*/ 2, 3, 3, 3,
/* 28*/ 4, 4, 5, 5,
/* 32*/ 6, 6, 7, 7,
/* 36*/ 7, 7, 1, 1,
/* 40*/ 1, 2, 2, 2,
/* 44*/ 3, 3, 3, 4,
/* 48*/ 4, 5, 5, 6,
/* 52*/ 6, 7, 7, 7,
/* 56*/ 7, 1, 1, 2,
/* 60*/ 2, 3, 3, 4,
/* 64*/ 4, 5, 5, 6,
/* 68*/ 6, 7, 7, 1,
/* 72*/ 1, 2, 3, 4,
/* 76*/ 5, 6, 7
};

uint8_t *spellAbbr[] = {
/*  0*/ "MAFL", "ARFI", "TRZP", "FRFO",
/*  4*/ "MACO", "WOHL", "LERE", "LEVI",
/*  8*/ "WAST", "INWO", "FLRE", "GRRE",
/* 12*/ "SHSP", "INOG", "MALE", "FLAN",
/* 16*/ "APAR", "FAFO", "INSL", "VOPL",
/* 20*/ "QUFI", "SCSI", "HOWA", "MAGA",
/* 24*/ "AREN", "MYSH", "OGST", "STFL",
/* 28*/ "SPTO", "DRBR", "ANMA", "STTO",
/* 32*/ "PHDO", "YMCA", "REST", "DEST",
/* 36*/ "WZWA", "SASP", "MIJA", "PHBL",
/* 40*/ "LOTR", "DISB", "WIWA", "FEAR",
/* 44*/ "WIOG", "INVI", "SESI", "CAEY",
/* 48*/ "WIDR", "DIIL", "MIBL", "WIGI",
/* 52*/ "SOSI", "WIMA", "WIHE", "MAGM",
/* 56*/ "ZZGO", "SUEL", "FOFO", "GATE",
/* 60*/ "DEBA", "FLCO", "DISP", "PRSU",
/* 64*/ "ANDE", "SPBI", "SOWH", "GRSU",
/* 68*/ "BEDE", "WIZW", "HERB", "HAFO",
/* 72*/ "MEME", "BASP", "CAMR", "NILA",
/* 76*/ "HEAL", "BRKR", "MAMA"
};

uint8_t *spellFull[] = {
/*   */  "MAGE FLAME", "ARC FIRE", "TRAP ZAP", "FREEZE FOES",
/*   */ "MAGIC COMPASS", "WORD OF HEALING", "LESSER REV.", "LEVITATION",
/*   */ "WARSTRIKE", "INSTANT WOLF", "FLESH RESTORE", "GREATER REV.",
/*   */ "SHOCK-SPHERE", "INSTANT OGRE", "MAJOR LEV.", "FLESH ANEW",
/*   */ "APPORT ARCANE", "FAR FOE", "INSTANT SLAYER", "VORPAL PLATING",
/*   */ "QUICK FIX", "SCRY SITE", "HOLY WATER", "MAGE GAUNTLETS",
/*   */ "AREA ENCHANT", "MYSTIC SHIELD", "OGRESTRENGTH", "STARFLARE",
/*   */ "SPECTRE TOUCH", "DRAGON BREATH", "ANTI-MAGIC", "STONE TOUCH",
/*   */ "PHASE DOOR", "MYSTICAL ARMOR", "RESTORATION", "DEATHSTRIKE",
/*   */ "WIZARD WALL", "SAFETY SPELL", "MIND JAB", "PHASE BLUR",
/*   */ "LOCATE TRAPS", "DISBELIEVE", "WIND WARRIOR", "WORD OF FEAR",
/*   */ "WIND OGRE", "INVISIBILITY", "SECOND SIGHT", "CAT EYES",
/*   */ "WIND DRAGON", "DISRUPT ILL.", "MIND BLADE", "WIND GIANT",
/*   */ "SORCERER SIGHT", "WIND MAGE", "WIND HERO", "MAGE MAELSTROM",
/*   */ "DREAMSPELL", "SMN ELEMENTAL", "FORCE FOCUS", "GATE",
/*   */ "DEMON BANE", "FLAME COLUMN", "DISPOSSESS", "PRIME SUMMONING",
/*   */ "ANIMATE DEAD", "SPELL BIND", "SOUL WHIP", "GREATER SUMMON",
/*   */ "BEYOND DEATH", "WIZARD WAR", "SUMMON HERB", "OSCON'S HALTFOE",
/*   */ "MELEE MEN", "BATCHSPELL", "CAMARADERIE", "NIGHT LANCE",
/*   */ "HEAL ALL", "BROTHER KRINGLE", "MANGAR'S MALLET"
};

uint8_t spptReq[] = {
  2,3,2,3,3,4,5,4,5,6,6,7,7,9,8,12,15,18,12,3,3,2,4,5,
  5,6,6,6,8,7,8,8,9,10,12,14,11,30,3,2,2,4,5,4,6,6,6,
  7,12,8,10,13,11,14,16,40,100,10,11,12,11,14,12,15,14,
  16,13,22,18,16,25,15,20,28,26,30,50,60,80,0
};

uint8_t spellAttr[] = {
  0x1C,0x0B,0x14,0x0A,0x1C,0x19,0x1C,0x1D,
  0x8A,0x1C,0x99,0x1C,0x4A,0x1C,0x1D,0x9D,
  0x15,0x0A,0x1C,8,0x19,0x14,0x6B,8,
  0x5C,0x1D,9,0x0A,0x6B,0x0A,0x0D,0x0B,
  0x14,0x1D,0x9D,0x0B,0x1C,0x1C,0x8B,0x0D,
  0x1C,0x0D,0x1C,0x0A,0x1C,0x0D,0x9C,0x1D,
  0x1C,0x0D,0x8E,0x1C,0x9C,0x1C,0x1C,0x0A,
  0xFE,0x1D,0xEA,0x1D,0x2B,0x0A,0xD8,0x1D,
  8,0x0A,0x6B,0x1D,0xF8,0x4A,0x1D,0x0E,
  0x0A,0x1D,0x0D,0x2A,0xBD,0x1D,0xEE,0,
  0x0A,0x2A,0x4A,0x8A,0x8A,0xAA,0x0A,0xCA,
  0xEA,0x0A,0x2A,0x4A,0x6A,0xAA,0xCA,0xEA,
  0x1C,0x1C,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
  0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D
};

uint8_t spellType[] = {
/*  0*/ sp_Lightspell,sp_ArcFire,sp_TrapZap,sp_FreezeFoes,
/*  4*/ sp_MagicCompass,sp_WordOfHealing,sp_Lightspell,sp_Levitation,
/*  8*/ sp_WarStrike,sp_Summon,sp_WordOfHealing,sp_Lightspell,
/* 12*/ sp_WarStrike,sp_Summon,sp_Levitation,sp_WordOfHealing,
/* 16*/ sp_Teleport,sp_FarFoe,sp_Summon,sp_VorpalPlating,
/* 20*/ sp_QuickFix,sp_ScrySight,sp_HolyWater,sp_VorpalPlating,
/* 24*/ sp_AreaEnchant,sp_GroupShield,sp_BattleSkill,sp_WarStrike,
/* 28*/ sp_SpectreTouch,sp_WarStrike,sp_AntiMagic,sp_StoneTouch,
/* 32*/ sp_PhaseDoor,sp_GroupShield,sp_WordOfHealing,sp_StoneTouch,
/* 36*/ sp_Summon,sp_SafetySpell,sp_ArcFire,sp_PhaseBlur,
/* 40*/ sp_AreaEnchant,sp_Disbelieve,sp_Summon,sp_WordOfFear,
/* 44*/ sp_Summon,sp_PhaseBlur,sp_AreaEnchant,sp_Lightspell,
/* 48*/ sp_Summon,sp_Disbelieve,sp_MindBlade,sp_Summon,
/* 52*/ sp_AreaEnchant,sp_Summon,sp_Summon,sp_HolyWater,
/* 56*/ sp_DreamSpell,sp_Summon,sp_WarStrike,sp_Summon,
/* 60*/ sp_HolyWater,sp_WarStrike,sp_WordOfHealing,sp_Summon,
/* 64*/ sp_StoneTouch,sp_Spellbind,sp_SpectreTouch,sp_Summon,
/* 68*/ sp_WordOfHealing,sp_WarStrike,sp_Summon,sp_HaltFoe,
/* 72*/ sp_MeleeMen,sp_BatchSpell,sp_Camaraderie,sp_WarStrike,
/* 76*/ sp_WordOfHealing,sp_Summon,sp_MindBlade,sp_None,
/* 80*/ sp_None,sp_WarStrike,sp_None,sp_None,
/* 84*/ sp_WarStrike,sp_WarStrike,sp_WarStrike,sp_None,
/* 88*/ sp_None,sp_None,sp_None,sp_None,
/* 92*/ sp_None,sp_WarStrike,sp_None,sp_None,
/* 96*/ sp_Lightspell,sp_Lightspell,sp_SpectreTouch,sp_SpectreTouch,
/*100*/ sp_SpectreTouch,sp_SpectreTouch,sp_SpectreTouch,sp_SpectreTouch,
/*104*/ sp_SpectreTouch,sp_SpectreTouch,sp_SpectreTouch,sp_SpectreTouch,
/*108*/ sp_SpectreTouch,sp_SpectreTouch,sp_SpectreTouch,sp_SpectreTouch,
/*112*/ sp_None,sp_None,sp_None,sp_None,
/*116*/ sp_None,sp_None,sp_None,sp_None
}; 

uint8_t spellDuration[] = {
/*  0*/ 0,1,0,0,
/*  4*/ 0x28,4,1,0,
/*  8*/ 5,0,0x0A,2,
/* 12*/ 0x0A,1,1,0x8A,
/* 16*/ 0,0,0x0D,1,
/* 20*/ 0x11,0,6,2,
/* 24*/ 0x14,0x28,2,8,
/* 28*/ 0x0F,0x0B,0,6,
/* 32*/ 0,0xFF,0x8F,7,
/* 36*/ 0x0E,0,2,0,
/* 40*/ 0x14,0xFF,0x82,0,
/* 44*/ 0x81,1,0x3C,3,
/* 48*/ 0x83,1,0x19,0x84,
/* 52*/ 0xFF,0x85,0x86,0x3C,
/* 56*/ 0xC8,7,0x19,8,
/* 60*/ 0x64,0x16,0,9,
/* 64*/ 5,0,0x32,0x0A,
/* 68*/ 0,0x32,0x0B,0,
/* 72*/ 0,0,0,0x64,
/* 76*/ 0x8F,0x0C,0xC8,0,
/* 80*/ 8,0x0A,0x0C,0x10,
/* 84*/ 0x18,0x20,0x30,0x40,
/* 88*/ 0x60,0x70,0x80,0xA0,
/* 92*/ 0xC0,0xD0,0xE0,0xF0,
/* 96*/ 6,7,3,5,
/*100*/ 6,4,8,9,
/*104*/ 0x0C,0x10,0x18,0x20,
/*108*/ 0x28,0x40,0x60,0x80,
/*112*/ 0x0F,0x10,0x11,0x12,
/*116*/ 0x13,0x14,0x15,0x16
};

uint8_t spellRange[] = {
/*  0*/ 0,1,0,0,
/*  4*/ 0,0,0,0,
/*  8*/ 0x82,0,0,0,
/* 12*/ 0x83,0,0,0,
/* 16*/ 0,0,0,0,
/* 20*/ 0,0,1,0,
/* 24*/ 0,0,0,0x84,
/* 28*/ 7,0x83,0,1,
/* 32*/ 0,0,0,1,
/* 36*/ 0,0,0x84,0,
/* 40*/ 0,0,0,0,
/* 44*/ 0,0,0,0,
/* 48*/ 0,0,0x83,0,
/* 52*/ 0,0,0,0x0A,
/* 56*/ 0x0A,0,1,0,
/* 60*/ 3,0x83,0,0,
/* 64*/ 0,0,7,0,
/* 68*/ 0,5,0,0,
/* 72*/ 0,0,0,6,
/* 76*/ 0,0,0x0A,0,
/* 80*/ 2,2,3,3,
/* 84*/ 4,5,5,6,
/* 88*/ 6,7,7,8,
/* 92*/ 8,8,9,9,
/* 96*/ 1,1,3,2,
/*100*/ 2,3,4,5,
/*104*/ 4,6,9,7,
/*108*/ 7,8,8,9
};

uint8_t	spellSummonMap[] = {
	0, 1, 2, 3,
	4, 5, 6, 7,
	8, 9, 10, 11,
	12, 13, 14, 4,
	15, 16, 13, 17,
	18, 5, 11, 12
};

uint8_t lightDist[] = {
  3,4,5,5,3,5,2,2,2,3,4,5
};

uint8_t lightDur[] = {
  0x28,0x3C,0x50,0xFF,0x1E,0x3C,0x1C,0x24,
  0x40,0x48,0x50,0x58 
};

uint8_t lightSDFlag[] = {
  0,0x3C,0x50,0,0,0x3C,0,0,
  0,0x10,0x18,0x20     
};

uint8_t levitDur[] = {
  0x14, 0xff
};

uint8_t phaseBlur[] = {
  1,4,3,0
};

uint8_t shieldSelf[] = {
  1,5,1,0
};

uint8_t wordoffear[] = {
  1,3
};

uint8_t battleskill[] = {
  4,10,7,0
};
