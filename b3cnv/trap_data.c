#include <b3lib.h>

uint8_t *dun_trap_strings[] = {
  "stone block!",
  "tripwire!",
  "pit!",
  "spiked pit!",
  "poison gas cloud!",
  "punji stakes!",
  "crossbow bolts!",
  "shock wave!",
  "acid bath!",
  "mind zap!",
  "poison spray!",
  "poison spiked pit!",
  "mage zap!",
  "decapitator!",
  "timed warstrike!",
  "shock sphere!",
  "crushing walls!",
  "rolling ball!",
  "basilisk snare!",
  "wither strike!",
  "sledgehammer!",
  "earthquake!",
  "deathstrike!",
  "bonkers!"
};

uint8_t *dun_trap_macros[] = {
  "DT_STONEBLOCK",
  "DT_TRIPWIRE",
  "DT_PIT",
  "DT_SPIKEDPIT",
  "DT_POISONGASCLOUD",
  "DT_PUNJISTAKES",
  "DT_CROSSBOWBOLTS",
  "DT_SHOCKWAVE",
  "DT_ACIDBATH",
  "DT_MINDZAP",
  "DT_POISONSPRAY",
  "DT_POISONSPIKEDPIT",
  "DT_MAGEZAP",
  "DT_DECAPITATOR",
  "DT_TIMEDWARSTRIKE",
  "DT_SHOCKSPHERE",
  "DT_CRUSHINGWALLS",
  "DT_ROLLINGBALL",
  "DT_BASILISKSNARE",
  "DT_WITHERSTRIKE",
  "DT_SLEDGEHAMMER",
  "DT_EARTHQUAKE",
  "DT_DEATHSTRIKE",
  "DT_BONKERS"
};

uint8_t dun_trap_map[] = {
/*  0*/ 0, 1, 2,
/*  4*/ 4, 5, 6,
/*  8*/ 8, 9, 10,
/* 12*/ 12, 13, 14,
/* 16*/ 16, 17, 18,
/* 20*/ 20, 21, 22,
/* 24*/ 24, 25, 26,
/* 28*/ 28, 29, 30
};

uint8_t dun_trap_spAttack[] = {
  0, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,
  3, 1, 1, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  6, 4, 0, 0,
  0, 7, 4, 0
};

uint8_t dun_trap_ndice[] = {
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0
};

uint8_t dun_trap_save[] = {
  0x0F, 0x0F, 0x0E, 0,
  0x13, 0x13, 0x13, 0,
  0x15, 0x14, 0x14, 0,
  0x18, 0x18, 0x17, 0,
  0x26, 0x26, 0x28, 0,
  0x27, 0x28, 0x28, 0,
  0x2A, 0x29, 0x2A, 0,
  0x2B, 0x2E, 0x2B, 0
};

uint8_t *chest_trap_strings[] = {
  "Poison Needle", "Poison Blades", "Blades", "Shock Wave",
  "Poison Needle", "Poison Blades", "Shock Wave", "Crazycloud",
  "Poison Needle", "Poison Blades", "Shocks", "Vortex",
  "Poison Darts", "Acid Burst", "Gas Cloud",
  "Poison Spikes", "Mind Blast", "Mind Jab", "Basilisk Snare",
  "Death Blades", "Codger Bomb", "Swindler", "Hammer"
};

uint8_t chest_trap_map[] = {
  0, 1, 2, 3,
  4, 5, 6, 7,
  8, 9, 10, 11,
  12, 13, 14, 14,
  15, 16, 17, 18,
  19, 20, 21, 22,
  19, 19, 19, 19,
  19, 19, 19, 19
};

uint8_t *chest_trap_macros[] = {
/*  0*/ "CT_POISONNEEDLE_0", "CT_POISONBLADES_0", "CT_BLADES_0", "CT_SHOCKWAVE_0",
/*  4*/ "CT_POISONNEEDLE_1", "CT_POISONBLADES_1", "CT_SHOCKWAVE_1", "CT_CRAZYCLOUD_0",
/*  8*/ "CT_POISONNEEDLE_2", "CT_POISONBLADES_2", "CT_SHOCKS_0", "CT_VORTEX_0",
/* 12*/ "CT_POISONDARTS_0", "CT_ACIDBURST_0", "CT_GASCLOUD_0", 
/* 15*/ "CT_POISONSPIKES_0", "CT_MINDBLAST_0", "CT_MINDJAB_0", "CT_BASILISKSNARE_0",
/* 19*/ "CT_DEATHBLADES_0", "CT_CODGERBOMB_0", "CT_SWINDLER_0", "CT_HAMMER_0"
};

uint8_t chest_trap_dice[] = {
/*  0*/ 0x41, 0x41, 0x44, 0x41,
/*  4*/ 0x43, 0x43, 0x46, 0x42,
/*  8*/ 0x45, 0x45, 0x47, 0x46,
/* 12*/ 0x47, 0x48, 0x47, 
/* 15*/ 0x49, 0x47, 0x49, 0x21,
/* 19*/ 0x49, 0x47, 0x45, 0x49
};

uint8_t chest_trap_flags[] = {
/*  0*/ 1, 1, 0, 0x80,
/*  4*/ 1, 1, 0x80, 3,
/*  8*/ 1, 1, 0x80, 9,
/* 12*/ 0x81, 0x80, 0x81, 
/* 15*/ 1, 3, 0, 6,
/* 19*/ 0x81, 0x84, 8, 0x80
};

uint8_t chest_trap_save[] = {
/*  0*/ 0x0F, 0x0F, 0x0F, 0x0F,
/*  4*/ 0x13, 0x13, 0x13, 0x13,
/*  8*/ 0x15, 0x15, 0x15, 0x15,
/* 12*/ 0x18, 0x18, 0x18,
/* 15*/ 0x26, 0x26, 0x26, 0x26,
/* 19*/ 0x27, 0x28, 0x27, 0x28
};

