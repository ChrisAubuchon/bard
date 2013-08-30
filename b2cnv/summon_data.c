#include <b2lib.h>
#include <mon.h>


b2mon_t sumMons[] = {
  { "Wolf", 		  63,  8, 0, 2, 8, 0,{  0,  0,  0,  0}, 0, 8,62,17 },
  { "Ogre",		  63, 20, 0, 1,10, 1,{  0,  0,  0,  0}, 0,10,55,17 },
  { "Ninja",		  31, 20, 0, 1,12, 1,{  0,  0,  0,  0}, 0, 9,59,16 },
  { "Red Dragon",	  63, 64, 0, 2,16, 2,{ 85, 85, 85, 86}, 0,24,32,81 },
  { "Storm Giant",	  63, 96, 0, 1,18, 2,{  0,  0,  0,  0}, 0,31,36,16 },
  { "Master Mage",	  63, 16, 0, 1,16, 0,{ 71, 75, 63, 78}, 0, 4,57,112},
  { "Samson",		 127, 32, 0, 1,21, 4,{  0,  0,  0,  0}, 0,36,46,16 },
  { "Fire Elemental",	  63,128, 0, 1,18, 2,{  0,  0,  0, 86}, 0,20,54,17 },
  { "Wraith", 		  63, 48, 0, 1,16, 2,{  0,  0,  0,  0}, 0,24,41,17 },
  { "Spectre", 		 127, 32, 0, 1,18, 3,{  0,  0,  0,  0}, 0,32,35,17 },
  { "Salamander",	 127, 68, 0, 1,19, 2,{  0,  0,  0, 87}, 0,34,54,17 },
  { "Herb",		 511,208, 0, 1,26, 1,{ 70, 66, 66, 66}, 0,32,55,96 },
  { "Kringle Bro",	1023,128, 0, 1,29, 3,{  0,  0,  0, 88}, 0,64,47,16 },
  { "Slayer",		  63, 64, 0, 1,23, 2,{  0,  0,  0,  0}, 7,31,52,17 },
  { "Wall",		 127, 64, 0, 0,20, 0,{255,255,255,255}, 0, 0,39,17 },
  { "Molten Man",	 127,128, 0, 1,14, 2,{  0,  0, 83, 83},96,16,54,17 },
  { "Bulldozer",	 255, 48, 0, 1,17, 4,{  0,  0,  0,107},80,16,38,16 },
  { "Vanquisher",	 511, 64, 0, 1,21, 7,{  0,  0,  0, 87}, 6,15,52,17 },
  { "Blast Dragon",	1023, 32, 0, 1,24, 3,{  0,  0, 92, 92},39,18,32,17 }
};

uint8_t *sumMacro[] = {
/*   */ "M_SUM_WOLF","M_SUM_OGRE","M_SUM_NINJA","M_SUM_RED_DRAGON",
/*   */ "M_SUM_STORM_GIANT","M_SUM_MASTER_MAGE","M_SUM_SAMSON","M_SUM_FIRE_ELEMNTAL",
/*   */ "M_SUM_WRAITH","M_SUM_SPECTRE","M_SUM_SALAMANDER","M_SUM_HERB",
/*   */ "M_SUM_KRINGLE_BRO","M_SUM_SLAYER","M_SUM_WALL","M_SUM_STORM_GIANT",
/*   */ "M_SUM_MOLTEN_MAN","M_SUM_BULLDOZER","M_SUM_SLAYER","M_SUM_VANQUISHER",
/*   */ "M_SUM_BLAST_DRAGON","M_SUM_MASTER_MAGE","M_SUM_HERB","M_SUM_KRINGLE_BRO"
};

