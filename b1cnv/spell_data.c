#include <b1lib.h>
#include <spell.h>

uint8_t *spellClass[] = {
/*  0*/	"Conjurer", "Conjurer", "Conjurer", "Conjurer",
/*  4*/	"Conjurer", "Conjurer", "Conjurer", "Conjurer",
/*  8*/	"Conjurer", "Conjurer", "Conjurer", "Conjurer",
/* 12*/	"Conjurer", "Conjurer", "Conjurer", "Conjurer",
/* 16*/	"Conjurer", "Conjurer", "Conjurer", "Conjurer",
/* 20*/	"Conjurer", "Conjurer", "Sorcerer", "Sorcerer",
/* 24*/	"Sorcerer", "Sorcerer", "Sorcerer", "Sorcerer",
/* 28*/	"Sorcerer", "Sorcerer", "Sorcerer", "Sorcerer",
/* 32*/	"Sorcerer", "Sorcerer", "Sorcerer", "Sorcerer",
/* 36*/	"Sorcerer", "Sorcerer", "Sorcerer", "Sorcerer",
/* 40*/	"Sorcerer", "Sorcerer", "Sorcerer", "Sorcerer",
/* 44*/	"Magician", "Magician", "Magician", "Magician", 
/* 48*/	"Magician", "Magician", "Magician", "Magician", 
/* 52*/	"Magician", "Magician", "Magician", "Magician", 
/* 56*/	"Magician", "Magician", "Magician", "Magician", 
/* 60*/	"Magician", "Magician", "Magician", "Magician", 
/* 64*/	"Magician", "Magician", "Wizard", "Wizard", 
/* 68*/	"Wizard", "Wizard", "Wizard", "Wizard", 
/* 72*/	"Wizard", "Wizard", "Wizard", "Wizard", 
/* 76*/	"Wizard", "Wizard", "Wizard"
};

uint8_t spellLevel[] = {
/*  0*/	1,1,1,1,
/*  4*/	2,2,2,2,
/*  8*/	3,3,3,3,
/* 12*/	4,4,4,5,
/* 16*/	5,5,6,6,
/* 20*/	7,7,1,1,
/* 24*/	1,1,2,2,
/* 28*/	2,2,3,3,
/* 32*/	3,3,4,4,
/* 36*/	4,5,5,5,
/* 40*/	6,6,7,7,
/* 44*/	1,1,1,1,
/* 48*/	2,2,2,2,
/* 52*/	3,3,3,3,
/* 56*/	4,4,4,5,
/* 60*/	5,5,6,6,
/* 64*/	7,7,1,1,
/* 68*/	2,2,3,3,
/* 72*/	4,4,5,5,
/* 76*/	6,6,7
};

uint8_t *spellAbbr[] = {
/*  0*/	"MAFL", "ARFI", "SOSH", "TRZP",
/*  4*/	"FRFO", "MACO", "BASK", "WOHL",
/*  8*/	"MAST", "LERE", "LEVI", "WAST",
/* 12*/	"INWO", "FLRE", "POST", "GRRE",
/* 16*/	"WROV", "SHSP", "INOG", "MALE",
/* 20*/	"FLAN", "APAR", "MIJA", "PHBL",
/* 24*/	"LOTR", "HYIM", "DISB", "TADU",
/* 28*/	"MIFI", "FEAR", "WIWO", "VANI",
/* 32*/	"SESI", "CURS", "CAEY", "WIWA",
/* 36*/	"INVI", "WIOG", "DIIL", "MIBL",
/* 40*/	"WIDR", "MIWP", "WIGI", "SOSI",
/* 44*/	"VOPL", "AIAR", "STLI", "SCSI",
/* 48*/	"HOWA", "WIST", "MAGA", "AREN",
/* 52*/	"MYSH", "OGST", "MIMI", "STFL",
/* 56*/	"SPTO", "DRBR", "STSI", "ANMA",
/* 60*/	"ANSW", "STTO", "PHDO", "YMCA",
/* 64*/	"REST", "DEST", "SUDE", "REDE",
/* 68*/	"LESU", "DEBA", "SUPH", "DISP",
/* 72*/	"PRSU", "ANDE", "SPBI", "DMST",
/* 76*/	"SPSP", "BEDE", "GRSU"
};

uint8_t *spellFull[] = {
	"MAGE FLAME", "ARC FIRE", "SORCERER SHIELD", "TRAP ZAP",
	"FREEZE FOES", "MAGIC COMPASS", "BATTLESKILL", "WORD OF HEALING",
	"MAGESTAR", "LESSER REV.", "LEVITATION", "WARSTRIKE",
	"INSTANT WOLF", "FLESH RESTORE", "POISON STRIKE", "GREATER REV.",
	"WRATH OF VAL.", "SHOCK-SPHERE", "INSTANT OGRE", "MAJOR LEV.",
	"FLESH ANEW", "APPORT ARCANE", "MIND JAB", "PHASE BLUR",
	"LOCATE TRAPS", "HYPNOTIC IMAGE", "DISBELIEVE", "TARGET-DUMMY",
	"MIND FIST", "WORD OF FEAR", "WIND WOLF", "VANISHING SPELL",
	"SECOND SIGHT", "CURSE", "CAT EYES", "WIND WARRIOR",
	"INVISIBILITY", "WIND OGRE", "DISRUPT ILL.", "MIND BLADE",
	"WIND DRAGON", "MIND WARP", "WINDGIANT", "SORCERER SIGHT",
	"VORPAL PLATING", "AIR ARMOR", "STEELIGHT SPELL", "SCRY SITE",
	"HOLY WATER", "WITHER STRIKE", "MAGE GAUNTLETS", "AREA ENCHANT",
	"MYSTIC SHIELD", "OGRESTRENGTH", "MITHRIL MIGHT", "STARFLARE",
	"SPECTRE TOUCH", "DRAGON BREATH", "STONELIGHT", "ANTI-MAGIC",
	"ANIMATED SWORD", "STONE TOUCH", "PHASE DOOR", "MYSTICAL ARMOR",
	"RESTORATION", "DEATHSTRIKE", "SUMMON DEAD", "REPEL DEAD",
	"LESSER SUMMON", "DEMON BANE", "SUMMON PHANTOM", "DISPOSSESS",
	"PRIME SUMMONING", "ANIMATE DEAD", "SPELL BIND", "DEMON STRIKE",
	"SPELL SPIRIT", "BEYOND DEATH", "GREATER SUMMON"
};

uint8_t spptReq[] = {
	2, 3, 3, 2, 3, 3, 4, 4,
	5, 5, 4, 5, 6, 6, 6, 7,
	7, 7, 9, 8, 12, 15, 3, 2,
	2, 3, 4, 4, 4, 4, 5, 6,
	6, 5, 7, 6, 7, 7, 8, 8,
	10, 9, 12, 11, 3, 3, 2, 2,
	4, 5, 5, 5, 6, 6, 7, 6,
	8, 7, 7, 8, 8, 8, 9, 10,
	12, 14, 6, 4, 8, 8, 10, 10,
	12, 11, 14, 13, 15, 18, 22
};

uint8_t spellAttr[] = {
	0x1C, 0x0B, 0x0C, 0x14, 0x0A, 0x1C, 8, 0x19,
	0x0A, 0x1C, 0x1D, 0x8A, 0x1C, 0x99, 0x0B, 0x1C,
	8, 0x4A, 0x1C, 0x1D, 0x9D, 0x14, 0x8B, 0x0D,
	0x1C, 0x0A, 0x0D, 0x0C, 0x8B, 0x0A, 0x1C, 0x0C,
	0x9C, 0x0A, 0x1D, 0x1C, 0x0D, 0x1C, 0x0D, 0x8E,
	0x1C, 0x0B, 0x1C, 0x9C, 8, 0x0C, 0x1C, 0x14,
	0x6B, 0x0B, 8, 0x5C, 0x1D, 9, 0x0D, 0x0A,
	0x6B, 0x0A, 0x1C, 0x0D, 0x0D, 0x0B, 0x14, 0x1D,
	0x9D, 0x0B, 0x1D, 0x0A, 0x1D, 0x2B, 0x1D, 0xD8,
	0x1D, 0x18, 0x0A, 0x2A, 0x1D, 0xF8, 0x1D, 0x2A,
	0x0A, 0x0A, 0x0A, 0x4A, 0x6A, 0x6A, 0x0A, 0x8A,
	0xAA, 0xCA, 0x1C, 0x1C, 0x1D, 0x1D, 0x1D, 0x1D,
	0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D,
	0x1D, 0
};

uint8_t spellDuration[] = {
/*   */ 0, 1, 0, 0,
/*   */ 0, 0x28, 0, 2,
/*   */ 0, 1, 0, 4,
/*   */ 0, 6, 1, 2,
/*   */ 1, 8, 1, 1,
/*   */ 0x86, 0, 2, 0,
/*   */ 0x14, 0, 1, 0x82,
/*   */ 3, 0, 0x80, 1,
/*   */ 0x3C, 1, 3, 0x83,
/*   */ 1, 0x81, 0xFF, 0x0A,
/*   */ 0x84, 3, 0x85, 0xFF,
/*   */ 1, 2, 4, 0,
/*   */ 6, 4, 2, 0x14,
/*   */ 0x28, 2, 2, 6,
/*   */ 0x0C, 8, 5, 0,
/*   */ 6, 6, 0, 0xFF,
/*   */ 0x8F, 7, 0x47, 0x10,
/*   */ 9, 0x20, 0x4A, 0,
/*   */ 0x0C, 5, 0, 0x20,
/*   */ 0x4D, 0, 0x4F, 0x18,
/*   */ 0x20, 0x28, 0x30, 0x22,
/*   */ 0x2C, 0x40, 0x48, 0x10,
/*   */ 0x12, 0x14, 6, 7,
/*   */ 0x11, 0x12, 0x13, 0x14,
/*   */ 0x15, 0x16, 0x17, 0x18,
/*   */ 0x19, 0x1A, 0x1B, 0x1C,
/*   */ 0x1D, 0
};

uint8_t spellType[] = {
/*  0*/ sp_LightSpell, sp_ArcFire, sp_ShieldSelf, sp_TrapZap,
/*  4*/ sp_FreezeFoes, sp_MagicCompass, sp_BattleSkill, sp_WordOfHealing,
/*  8*/ sp_MageStar, sp_LightSpell, sp_Levitation, sp_WarStrike,
/* 12*/ sp_InstantWolf, sp_WordOfHealing, sp_PoisonStrike, sp_LightSpell,
/* 16*/ sp_BattleSkill, sp_WarStrike, sp_InstantWolf, sp_Levitation,
/* 20*/ sp_WordOfHealing, sp_Teleport, sp_ArcFire, sp_PhaseBlur,
/* 24*/ sp_LocateTraps, sp_MageStar, sp_Disbelieve, sp_InstantWolf,
/* 28*/ sp_ArcFire, sp_WordOfFear, sp_InstantWolf, sp_ShieldSelf,
/* 32*/ sp_LocateTraps, sp_WordOfFear, sp_LightSpell, sp_InstantWolf,
/* 36*/ sp_PhaseBlur, sp_InstantWolf, sp_Disbelieve, sp_MindBlade,
/* 40*/ sp_InstantWolf, sp_PoisonStrike, sp_InstantWolf, sp_LocateTraps,
/* 44*/ sp_VorpalPlating, sp_ShieldSelf, sp_LightSpell, sp_ScrySight,
/* 48*/ sp_HolyWater, sp_PoisonStrike, sp_VorpalPlating, sp_LocateTraps,
/* 52*/ sp_GroupShield, sp_BattleSkill, sp_PhaseBlur, sp_WarStrike,
/* 56*/ sp_SpectreTouch, sp_WarStrike, sp_LightSpell, sp_AntiMagic,
/* 60*/ sp_InstantWolf, sp_PoisonStrike, sp_PhaseDoor, sp_GroupShield,
/* 64*/ sp_WordOfHealing, sp_PoisonStrike, sp_InstantWolf, sp_HolyWater,
/* 68*/ sp_InstantWolf, sp_HolyWater, sp_InstantWolf, sp_WordOfHealing,
/* 72*/ sp_InstantWolf, sp_PoisonStrike, sp_Spellbind, sp_HolyWater,
/* 76*/ sp_InstantWolf, sp_WordOfHealing, sp_InstantWolf, sp_WarStrike,
/* 80*/ sp_WarStrike, sp_WarStrike, sp_WarStrike, sp_WarStrike,
/* 84*/ sp_WarStrike, sp_WarStrike, sp_WarStrike, sp_WarStrike,
/* 88*/ sp_WarStrike, sp_WarStrike, sp_LightSpell, sp_LightSpell,
/* 92*/ sp_InstantWolf, sp_InstantWolf, sp_InstantWolf, sp_InstantWolf,
/* 96*/ sp_InstantWolf, sp_InstantWolf, sp_InstantWolf, sp_InstantWolf,
/*100*/ sp_InstantWolf, sp_InstantWolf, sp_InstantWolf, sp_InstantWolf,
/*104*/ sp_InstantWolf, sp_InstantWolf, sp_InstantWolf, sp_InstantWolf,
/*108*/ sp_InstantWolf, sp_InstantWolf, sp_InstantWolf, sp_InstantWolf,
/*112*/ sp_InstantWolf, sp_InstantWolf, sp_InstantWolf, sp_InstantWolf,
/*116*/ sp_InstantWolf, sp_InstantWolf, sp_InstantWolf, sp_InstantWolf,
/*120*/ sp_InstantWolf, sp_InstantWolf, sp_InstantWolf, sp_InstantWolf
};

uint8_t summonIndex[] = {
	16, 29, 0, 15,
	101, 121, 2, 10,
	20, 78, 51, 60,
	95, 124, 107, 111,
	126, 59, 85, 29,
	98, 79, 127, 107,
	4, 112, 124, 25,
	102, 93
};

uint8_t lightDist[] = {
	3, 4, 5, 5, 3, 5, 2, 2, 2, 3, 4, 5
};

uint8_t lightDur[] = {
	0x28, 0x3C, 0x50, 0xFF, 0x1E, 0x3C, 0x1C, 0x24, 0x20, 0x28, 0x30, 38
};

uint8_t lightSDFlag[] = {
	0, 0x3C, 0x50, 0, 0, 0x3C, 0, 0, 0, 0x10, 0x18, 0x20
};

uint8_t levitDur[] = {
	0x14, 0xff
};

uint8_t phaseBlur[] = {
	1, 4, 3, 0
};

uint8_t shieldSelf[] = {
	1, 5, 1, 0
};

uint8_t battleSkill[] = {
	4, 10, 7, 0
};

uint8_t wordoffear[] = {
	1, 3, 0
};

uint8_t songBonusList[] = {
	1, 1, 2, 2,
	2, 3, 3, 4
};

uint8_t *songName[] = {
	"Falkens Fury",
	"Seekers Ballad",
	"Waylands Watch",
	"Badhr Kilnfest",
	"Traveller tune",
	"Lucklaran"
};
