#include <b2lib.h>
#include <item.h>
#include <cnv_item.h>

uint8_t *itemName[] = {
  "Torch", "Lamp", "Broadsword", "Short Sword",
  "Dagger", "War Axe", "Halbard", "Long Bow",
  "Staff", "Buckler", "Tower Shield", "Leather Armor",
  "Chain Mail", "Scale Armor", "Plate Armor", "Robes",
  "Helm", "Leather Glvs.", "Gauntlets", "Mandolin",
  "Spear", "Arrows", "Mthr Sword", "Mthr Shield",
  "Mthr Chain", "Mthr Scale", "Giant Fgn", "Bracers [6]",
  "Bardsword", "Cold Horn", "Lightwand", "Mthr Dagger",
  "Mthr Helm", "Mthr Gloves", "Mthr Axe", "Shuriken",
  "Mthr Plate", "Molten Fgn", "Spell Spear", "Shield Ring",
  "Fin's Flute", "Kael's Axe", "Mthr Arrows", "Dayblade",
  "Shield Staff", "Elf Cloak", "Hawkblade", "Admt Sword",
  "Admt Shield", "Admt Helm", "Admt Gloves", "Pureblade",
  "Boomerang", "Ali's Carpet", "Luckshield", "Bulldozer Fgn",
  "Admt Chain", "Death Stars", "Admt Plate", "Bracers [4]",
  "Slayer Fgn", "Pure Shield", "Mage Staff", "War Staff",
  "Thief Dagger", "Soul Mace", "Ogrewand", "Kato's Bracer",
  "Sorcerstaff", "Galt's Flute", "Frost Horn", "Ag's Arrows",
  "Dmnd Shield", "Bard Bow", "Dmnd Helm", "Elf Boots", "Van Fgn",
  "Conjurstaff", "Staff of Lor", "Ring of Return", "Powerstaff",
  "Breathring", "Dragonshield", "Dmnd Plate", "Wargloves",
  "Wizhelm", "Dragonwand", "Deathring", "Crystal Sword",
  "Speedboots", "Flame Horn", "Zen Arrows", "Drums of Death",
  "Pipes of Pan", "Ring of Power", "Song Axe", "Trick Brick",
  "Dragon Fgn", "Mastermage Fgn", "Troll Ring", "Aram's Knife",
  "Angra's Eye", "Herb Fgn", "Master Wand", "Brothers Fgn",
  "Dynamite", "Thor's Hammer", "Stoneblade", "Grenade",
  "Master Key", "Nospen Ring", "Torch!", "Sword of Zar",
  "Vial", "Item of K", "The Ring", "Troy P.",
  "Dagger!", "Seg 1", "Seg 2", "Seg 3",
  "Seg 4", "Seg 5", "Seg 6", "Seg 7",
  "The Scepter", "Spectre Snare"
};

uint8_t itemType[] = {
/*  0*/ 0,0,1,1,
/*  4*/ 1,1,1,0x0B,
/*  8*/ 1,2,2,3,
/* 12*/ 3,3,3,3,
/* 16*/ 4,5,5,6,
/* 20*/ 1,12,1,2,
/* 24*/ 3,3,7,3,
/* 28*/ 1,6,9,1,
/* 32*/ 4,5,1,12,
/* 36*/ 3,7,1,8,
/* 40*/ 6,0x11,12,1,
/* 44*/ 1,14,1,1,
/* 48*/ 2,4,5,1,
/* 52*/ 12,0,2,7,
/* 56*/ 3,12,3,3,
/* 60*/ 7,2,1,1,
/* 64*/ 1,0x51,9,0,
/* 68*/ 1,6,6,12,
/* 72*/ 2,0x0B,4,0,
/* 76*/ 7,1,1,8,
/* 80*/ 1,8,2,3,
/* 84*/ 5,4,9,8,
/* 88*/ 1,0,6,12,
/* 92*/ 6,6,8,1,
/* 96*/ 0,7,7,8,
/*100*/ 1,0,7,9,
/*104*/ 7,12,1,0x61,
/*108*/ 12,0,8,0,
/*112*/ 1,0,0,8,
/*116*/ 8,0,0,0,
/*120*/ 0,0,0,0,
/*124*/ 0,0,0x71
};

uint8_t itemEquipMask[] = {
  0xFF,0xFF,0x8F,0xBF,0xFF,0x8F,0x87,0xBF,0xFF,0xBF,0x8E,
  0xBF,0x8F,0x8E,0x86,0xFF,0xBF,0xFF,0x86,8,0x9F,0xBF,0x9E,
  0xBF,0x8E,0x8E,0xFF,0xFF,8,8,0x60,0xFF,0x9F,0x8C,0x8F,0x9B,
  0x8C,0xFF,0xBF,0xFF,8,0x8E,0x9F,0x8E,0xFF,0xFF,0x8E,0x9E,
  0x9E,0x9E,0x84,4,0x99,0xFF,0x9E,0xFF,0x8E,2,0x84,0xFF,0xFF,
  4,0x60,0xFF,0x10,0x8E,0x60,0x60,0x60,8,8,0x86,0x8C,8,0x86,0xFF,
  8,0x60,0x61,0x64,0xFC,0xFF,0xFF,0x84,0x84,0x20,0x60,0x86,0x8E,
  0xBF,8,0x9F,8,8,0x62,8,0x60,0xFF,0xFF,0xFF,0x86,0x6C,0xFF,
  0x20,0xFF,0xFF,0x84,0x86,0xFF,0xFF,0xFF,0xFF,0x86,0xFF,
  0xFF,0xFF,0xFF,0xFF,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
  0x8E
};

uint8_t itemEffect[] = {
/*  0*/ 0,0,0,0,
/*  4*/ 0,0,0,0x0D,
/*  8*/ 0,0,0,0,
/* 12*/ 0,0,0,0,
/* 16*/ 0,0,0,0,
/* 20*/ 0,0x11,0,0,
/* 24*/ 0,0,0,0,
/* 28*/ 5,0x80,0,0,
/* 32*/ 0,0,0,0x80,
/* 36*/ 0,0,0x80,0,
/* 40*/ 0,0,0x11,0,
/* 44*/ 0,0,0,0,
/* 48*/ 0,0,0,0,
/* 52*/ 0,0,6,0,
/* 56*/ 0,0x80,0,0,
/* 60*/ 0,0,2,0,
/* 64*/ 8,0,0,9,
/* 68*/ 0x80,0,0x80,0x11,
/* 72*/ 0,0x0D,0,6,
/* 76*/ 0,4,0,0,
/* 80*/ 0x80,0x0A,0x80,0,
/* 84*/ 0,0x80,0x80,0x80,
/* 88*/ 0,7,0x80,0x11,
/* 92*/ 0x80,0,0x80,5,
/* 96*/ 0,0,0,1,
/*100*/ 0x80,0,0,0x80,
/*104*/ 0,0x80,0x81,0x80,
/*108*/ 0x80,0x0F,3,0,
/*112*/ 0x80,0x0E,0x10,0,
/*116*/ 0,0,0x20,0x21,
/*120*/ 0xA2,0x23,0x24,0x25,
/*124*/ 0x26,0xA7,0x80
};

uint8_t itemSpell[] = {
  0x60,0x61,0,0,0,0x64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x63,
  0x62,0,0,0,0,0x70,0,0,0x51,6,0,0,0,0x64,0x65,0,0x71,0x66,0,
  0,0x64,0x67,6,0,0,0,0,0,0,0,0,0x68,0x0E,0,0x72,0,0x69,0,0,
  0x73,0,0,0,0,0,0x0D,0,0x31,9,0x54,0x6A,0,0,0,0,0x74,0,0x22,
  0x25,8,0,0x54,0,0,0x45,0x55,0x40,0,0,0x56,0x6C,0x23,0x0B,0x32,
  0x6D,0x24,0x75,0x76,0,0x6F,0x49,0x77,0x4B,0x4D,0x4E,0x6B,0,0x4E,
  0,0,0,0x6E,0,0,0,0,0,0x24,0x21,0x45,0x35,0x49,0x4C,0x4D,0x5D,
  0x41
};

uint8_t itemEClass[] = {
/*  0*/ 0,0,1,1,
/*  4*/ 1,1,1,0x0B,
/*  8*/ 1,2,2,3,
/* 12*/ 3,3,3,3,
/* 16*/ 4,5,5,6,
/* 20*/ 1,0x0A,1,2,
/* 24*/ 3,3,7,3,
/* 28*/ 1,6,9,1,
/* 32*/ 4,5,1,0x0A,
/* 36*/ 3,7,1,8,
/* 40*/ 6,0x11,0x0A,1,
/* 44*/ 1,0x0A,1,1,
/* 48*/ 2,4,5,1,
/* 52*/ 0x0A,0x0A,2,7,
/* 56*/ 3,0x0A,3,3,
/* 60*/ 7,2,1,1,
/* 64*/ 1,0x51,9,0x0A,
/* 68*/ 1,6,6,0x0A,
/* 72*/ 2,0x0B,4,0x0A,
/* 76*/ 7,1,1,8,
/* 80*/ 1,8,2,3,
/* 84*/ 5,4,9,8,
/* 88*/ 1,0x0A,6,0x0A,
/* 92*/ 6,6,8,1,
/* 96*/ 0x0A,7,7,8,
/*100*/ 1,0x0A,7,9,
/*104*/ 7,0x0A,1,0x61,
/*108*/ 0x0A,0x0A,8,0,
/*112*/ 1,0x0A,0,8,
/*116*/ 8,0x0A,0x0A,0x0A,
/*120*/ 0x0A,0x0A,0x0A,0x0A,
/*124*/ 0x0A,0x0A,0x71
};


uint8_t itemCount[] = {
/*  0*/ 1,1,0xFF,0xFF,
/*  4*/ 0xFF,1,0xFF,0xFF,
/*  8*/ 0xFF,0xFF,0xFF,0xFF,
/* 12*/ 0xFF,0xFF,0xFF,0xFF,
/* 16*/ 0xFF,0xFF,0xFF,0xFF,
/* 20*/ 1,0x0A,0xFF,0xFF,
/* 24*/ 0xFF,0xFF,1,0xFF,
/* 28*/ 0xFF,0x32,0x50,0xFF,
/* 32*/ 0xFF,0xFF,0xFF,1,
/* 36*/ 0xFF,1,1,0xFF,
/* 40*/ 0xFF,1,0x0A,0x50,
/* 44*/ 0xFF,0xFF,0xFF,0xFF,
/* 48*/ 0xFF,0xFF,0xFF,0xFF,
/* 52*/ 0xFF,0x58,0xFF,1,
/* 56*/ 0xFF,4,0xFF,0xFF,
/* 60*/ 1,0xFF,0xFF,0xFF,
/* 64*/ 0xFF,0xFF,0x58,0xFF,
/* 68*/ 0x50,0x58,0x41,0x0A,
/* 72*/ 0xFF,0xFF,0xFF,0xFF,
/* 76*/ 1,0xFF,0x23,3,
/* 80*/ 0x33,0xFF,0x14,0xFF,
/* 84*/ 0xFF,0x19,0x23,0x40,
/* 88*/ 0xFF,0xFF,0x3C,0x0A,
/* 92*/ 0x0C,0xFF,0x11,0xFF,
/* 96*/ 0x1E,1,1,0xFF,
/*100*/ 0xFF,7,1,0x0B,
/*104*/ 1,1,0xFF,0xFF,
/*108*/ 1,0xFF,0xFF,0xFF,
/*112*/ 0xFF,0xFF,0xFF,0xFF,
/*116*/ 0xFF,0xFF,0xFF,0xFF,
/*120*/ 0xFF,0xFF,0xFF,0xFF,
/*124*/ 0xFF,0xFF,0x40
};

uint8_t itemDmgAc[] = {
/*  0*/ 0,0,0,0,
/*  4*/ 0,0,0,0,
/*  8*/ 0,1,2,2,
/* 12*/ 3,4,5,1,
/* 16*/ 1,1,1,0,
/* 20*/ 0,0,0x20,3,
/* 24*/ 4,5,0,4,
/* 28*/ 0x40,0,0,0x20,
/* 32*/ 2,2,0x20,0,
/* 36*/ 6,0,0x20,2,
/* 40*/ 2,0x20,0,0x10,
/* 44*/ 0x12,3,0x10,0x40,
/* 48*/ 4,3,3,0,
/* 52*/ 0,2,3,0,
/* 56*/ 5,0,7,6,
/* 60*/ 0,5,2,0,
/* 64*/ 0x10,0x30,0,0,
/* 68*/ 0x32,0,0,0x60,
/* 72*/ 5,2,4,0,
/* 76*/ 0,0x22,0x10,0,
/* 80*/ 0x10,0x10,4,8,
/* 84*/ 4,3,0,1,
/* 88*/ 0x10,0,0,0,
/* 92*/ 2,2,0,3,
/* 96*/ 0,0,0,0,
/*100*/ 0x40,0,0,0,
/*104*/ 0,0,0x20,0x30,
/*108*/ 0,0,0,0,
/*112*/ 0,0,0,0,
/*116*/ 0x44,0,0,0,
/*120*/ 0,0,0,0,
/*124*/ 0,0,0x19
};

uint8_t itemDmgDice[] = {
/*  0*/ 0,0,1,0x20,
/*  4*/ 0,1,0x40,0,
/*  8*/ 0x20,0,0,0,
/* 12*/ 0,0,0,0,
/* 16*/ 0,0,0,0,
/* 20*/ 0x20,0,1,0,
/* 24*/ 0,0,0,0,
/* 28*/ 0x21,0,0,0,
/* 32*/ 0,0,1,0,
/* 36*/ 0,0,0x40,0,
/* 40*/ 0,4,0,0x22,
/* 44*/ 0x40,0,0x22,1,
/* 48*/ 0,0,0,0x42,
/* 52*/ 0,0,0,0,
/* 56*/ 0,0,0,0,
/* 60*/ 0,1,0x22,0x23,
/* 64*/ 4,0x21,0,0,
/* 68*/ 2,0,0,1,
/* 72*/ 0,0,0,0,
/* 76*/ 0,2,4,0,
/* 80*/ 0x22,0x41,0,0,
/* 84*/ 0,0,0,0,
/* 88*/ 0x42,0,0,0,
/* 92*/ 0,0,0,0,
/* 96*/ 0,0,0,0,
/*100*/ 1,0,0,0,
/*104*/ 0,0,0x43,0,
/*108*/ 0,0,0,0,
/*112*/ 0x81,0,0,0,
/*116*/ 0,0,0,0,
/*120*/ 0,0,0,0,
/*124*/ 0,0,0x43
};

#if 0
uint8_t useWeap[] = {
  USE_MAKELIGHT, USE_MAKELIGHT,
  USE_SHOOTARROW, USE_THROWSPEAR,
  USE_THROWAXE, USE_HURLSHURI,
  USE_THROWSPEAR, USE_SHOOTARROW,
  USE_THROWBOOM, USE_HURLSHURI,
  USE_SHOOTARROW, USE_HURLHAMMER,
  USE_SHOOTARROW, USE_THROWAXE,
  USE_THROWBLADE, USE_THROWBLADE
};
#endif
uint8_t useWeap[] = {
	FIRE_ARROW,	FIRE_SPEAR,
	FIRE_AXE,	FIRE_SHURIKEN,
	FIRE_SPEAR,	FIRE_ARROW,
	FIRE_BOOMERANG,	FIRE_SHURIKEN,
	FIRE_ARROW,	FIRE_HAMMER,
	FIRE_ARROW,	FIRE_AXE,
	FIRE_BLADE,	FIRE_BLADE
};

uint8_t itemValue[] = {
/*  0*/ 40,120,65,25,
/*  4*/ 17,57,18,49,
/*  8*/ 17,33,10,57,
/* 12*/ 121,26,58,33,
/* 16*/ 41,65,33,105,
/* 20*/ 105,105,26,34,
/* 24*/ 42,74,42,50,
/* 28*/ 58,98,11,18,
/* 32*/ 26,34,26,26,
/* 36*/ 19,26,11,58,
/* 40*/ 10,106,50,58,
/* 44*/ 34,11,42,50,
/* 48*/ 42,42,26,34,
/* 52*/ 42,34,50,50,
/* 56*/ 11,66,98,66,
/* 60*/ 34,66,98,130,
/* 64*/ 11,19,19,43,
/* 68*/ 51,90,122,35,
/* 72*/ 67,27,59,12,
/* 76*/ 99,98,11,66,
/* 80*/ 90,26,50,67,
/* 84*/ 35,75,99,12,
/* 88*/ 12,28,44,66,
/* 92*/ 11,68,13,13,
/* 96*/ 42,60,12,52,
/*100*/ 11,11,44,98,
/*104*/ 100,11,11,11,
/*108*/ 19,122,21,40,
/*112*/ 13,40,8,10,
/*116*/ 0,40,13,13,
/*120*/ 13,13,13,13,
/*124*/ 13,61,21
};

uint8_t itemB1map[] = {
  0,1,2,3,
  4,5,6,7,
  0,9,10,11,
  12,13,14,15,
  16,17,18,19,
  20,0,0,23,
  24,25,26,0,
  28,29,0,31,
  32,33,34,35,
  0,37,0,0,
  40,0,41,42,
  0,44,45,46,
  47,48,49,0,
  50,51,0,0,
  52,0,54,0,
  55,34,57,0,
  59,60,0,62,
  63,64,65,66,
  0,69,0,0,
  70,71,0,73,
  0,75,0,0,
  78,0,79,81,
  0,83,84,85,
  0,87,0,90,
  91,0,0,94,
  95,88,0,0,
  0,0,67,0,
  98,0,100,0,
  0,89,108,0,
  0,0,0,0,
  110,0,0,0,
  0,0,0,127
};

uint8_t *itemB1name[] = {
  NULL,"Torch", "Lamp", "Broadsword", "Short Sword", "Dagger",
  "War Axe", "Halbard", "Mace", "Staff", "Buckler",
  "Tower Shield", "Leather Armor", "Chain Mail", "Scale Armor", "Plate Armor",
  "Robes", "Helm", "Leather Glvs.", "Gauntlets", "Mandolin",
  "Harp", "Flute", "Mthr Sword", "Mthr Shield", "Mthr Chain",
  "Mthr Scale", "Samurai Fgn", "Bracers [6]", "Bardsword", "Fire Horn",
  "Lightwand", "Mthr Dagger", "Mthr Helm", "Mthr Gloves", "Mthr Axe",
  "Mthr Mace", "Mthr Plate", "Ogre Fgn", "Lak's Lyre", "Shield Ring",
  "Dork Ring", "Fin's Flute", "Kael's Axe", "Blood Axe", "Dayblade",
  "Shield Staff", "Elf Cloak", "Hawkblade", "Admt Sword", "Admt Shield",
  "Admt Dagger", "Admt Helm", "Admt Gloves", "Admt Mace", "Broom",
  "Pureblade", "Exorwand", "Ali's Carpet", "Magic Mouth", "Luckshield",
  "Giant Fgn", "Admt Chain", "Admt Scale", "Admt Plate", "Bracers [4]",
  "Arcshield", "Pure Shield", "Mage Staff", "War Staff", "Thief Dagger",
  "Soul Mace", "Wither Staff", "Sorcerstaff", "Sword of Pak", "Heal Harp",
  "Galt's Flute", "Frost Horn", "Dmnd Sword", "Dmnd Shield", "Dmnd Dagger",
  "Dmnd Helm", "Golem Fgn", "Titan Fgn", "Conjurstaff", "Arc's Hammer",
  "Staff of Lor", "Powerstaff", "Mournblade", "Dragonshield", "Dmnd Plate",
  "Wargloves", "Lorehelm", "Dragonwand", "Kiels Compass", "Speedboots",
  "Flame Horn", "Truthdrum", "Spiritdrum", "Pipes of Pan", "Ring of Power",
  "Deathring", "Ybarrashield", "Spectre Mace", "Dag Stone", "Arc's Eye",
  "Ogrewand", "Spirithelm", "Dragon Fgn", "Mage Fgn", "Troll Ring",
  "Troll Staff", "Onyx Key", "Crystal Sword", "Stoneblade", "Travelhelm",
  "Death Dagger", "Mongo Fgn", "Lich Fgn", "Eye", "Master Key",
  "WizWand", "Silvr Square", "Silvr Circle", "Silvr Triang", "Thor Fgn",
  "Old Man Fgn", "Spectre Snare"
};
