#include <b3lib.h>
#include <dun.h>

#define DUN(nam,wflag,nlevs, levs)	\
  { nam,wflag,nlevs,{levs} }

#define LEVS(one,two,thr,for,fiv) \
  one,two,thr,for,fiv

#define LEV(map, offset) \
  { map, offset }

#define MAPLO(off) 	0, (off)
#define MAPHI(off) 	1, (off)
#define CODE(count,offset)	count, offset

#define LEV_NULL { 0, 0, 0, 0 }

uint8_t levno2dun[] = {
/*  0*/0, 1, 2, 3,
/*  4*/4, 16, 17, 18,
/*  8*/19, 20, 5, 5,
/* 12*/6, 7, 7, 7,
/* 16*/7, 8, 9, 9,
/* 20*/9, 9, 10, 10,
/* 24*/10, 10, 11, 11,
/* 28*/11, 11, 12, 12,
/* 32*/13, 13, 21, 21,
/* 36*/22, 22, 22, 23,
/* 40*/23, 24, 25, 26,
/* 44*/27, 28, 29, 30,
/* 48*/31, 32, 14, 14,
/* 52*/33, 33, 33, 34,
/* 56*/35, 36, 37, 38,
/* 60*/39, 40, 15, 15,
/* 64*/15, 15, 41, 42,
/* 68*/43, 43, 44 
};

uint8_t levno2lev[] = {
/*  0*/0, 0, 0, 0,
/*  4*/0, 0, 0, 0,
/*  8*/0, 0, 0, 1,
/* 12*/0, 0, 1, 2,
/* 16*/3, 0, 0, 1,
/* 20*/2, 3, 0, 1,
/* 24*/2, 3, 0, 1,
/* 28*/2, 3, 0, 1,
/* 32*/0, 1, 0, 1,
/* 36*/0, 1, 2, 0,
/* 40*/1, 0, 0, 0,
/* 44*/0, 0, 0, 0,
/* 48*/0, 0, 0, 1,
/* 52*/0, 1, 2, 0,
/* 56*/0, 0, 0, 0,
/* 60*/0, 0, 0, 1,
/* 64*/2, 3, 0, 0,
/* 68*/0, 1, 0 
};

uint8_t exitLocMap[] = {
/*  0*/ 0, 1, 2, 3,
/*  4*/ 4, 16, 17, 18,
/*  8*/ 19, 20
}; 

b3dunData_t duns[] = {
/* 0*/DUN( "wilderness", 1, 1,
	LEVS(	
		LEV(MAPLO(0), CODE(23, 0x22c)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL 
		)
  ),
/* 1*/DUN( "skara_brea", 1, 1,	
	LEVS(	
		LEV(MAPLO(1), CODE(10, 0x183)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL 
		)
  ),
/* 2*/DUN( "arboria", 1, 1,		
	LEVS(	
		LEV(MAPLO(2), CODE(29, 0x134)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL 
		)
  ),
/* 3*/DUN( "ciera_brannia", 1, 1,	
	LEVS(
		LEV(MAPLO(3), CODE(16, 0x178)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/* 4*/DUN( "gelidia", 1, 1,		
	LEVS(
		LEV(MAPLO(4), CODE(10, 0x158)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/* 5*/DUN( "festering_pit", 0, 2,	
	LEVS(
		LEV(MAPLO(5), CODE(4, 0x4b8)),
		LEV(MAPLO(6), CODE(3, 0x319)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/* 6*/DUN( "palace", 0, 1,		
	LEVS(
		LEV(MAPLO(7), CODE(8, 0x347)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/* 7*/DUN( "tower", 0, 4,		
	LEVS(
		LEV(MAPLO(8), CODE(4, 0xbc)),
		LEV(MAPLO(9), CODE(4, 0xbc)),
		LEV(MAPLO(10), CODE(8, 0xcc)),
		LEV(MAPLO(11), CODE(3, 0xb8)),
		LEV_NULL
		)
  ),
/* 8*/DUN( "sacred_grove", 0, 1,	
	LEVS(
		LEV(MAPLO(12), CODE(9, 0x251)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/* 9*/DUN( "white_tower", 0, 4,	
	LEVS(
		LEV(MAPLO(13), CODE(2, 0xb4)),
		LEV(MAPLO(14), CODE(2, 0xb4)),
		LEV(MAPLO(15), CODE(3, 0xb8)),
		LEV(MAPLO(16), CODE(3, 0xb8)),
		LEV_NULL
		)
  ),
/*10*/DUN( "grey_tower", 0, 4,	
	LEVS(
		LEV(MAPLO(17), CODE(2, 0xb4)),
		LEV(MAPLO(18), CODE(3, 0xb8)),
		LEV(MAPLO(19), CODE(3, 0xb8)),
		LEV(MAPLO(20), CODE(3, 0xb8)),
		LEV_NULL
		)
  ),
/*11*/DUN( "black_tower", 0, 4,	
	LEVS(
		LEV(MAPLO(21), CODE(2, 0xb4)),
		LEV(MAPLO(22), CODE(3, 0xb8)),
		LEV(MAPLO(23), CODE(3, 0xb8)),
		LEV(MAPLO(24), CODE(3, 0xb8)),
		LEV_NULL
		)
  ),
/*12*/DUN( "ice_dungeon", 0, 2,	
	LEVS(
		LEV(MAPLO(25), CODE(3, 0x1d8)),
		LEV(MAPLO(26), CODE(4, 0xbc)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*13*/DUN( "ice_keep", 0, 2,		
	LEVS(
		LEV(MAPLO(27), CODE(13, 0x2c5)),
		LEV(MAPLO(28), CODE(3, 0x29d)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*14*/DUN( "catacombs", 0, 2,		
	LEVS(
		LEV(MAPLO(29), CODE(6, 0x3a4)),
		LEV(MAPLO(30), CODE(7, 0x4a1)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*15*/DUN( "unterbrae", 0, 4,		
	LEVS(
		LEV(MAPLO(31), CODE(9, 0x4cc)),
		LEV(MAPLO(32), CODE(7, 0x4c4)),
		LEV(MAPLO(33), CODE(8, 0x4c8)),
		LEV(MAPLO(34), CODE(3, 0x4a9)),
		LEV_NULL
		)
  ),
/*16*/DUN( "lucencia", 1, 1,		
	LEVS(
		LEV(MAPHI(0), CODE(15, 0xfc)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*17*/DUN( "celaria_bree", 1, 1,	
	LEVS(
		LEV(MAPHI(1), CODE(10, 0x160)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*18*/DUN( "nowhere", 1, 1,		
	LEVS(
		LEV(MAPHI(2), CODE(9, 0xcb)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*19*/DUN( "dark_copse", 1, 1,	
	LEVS(
		LEV(MAPHI(3), CODE(4, 0xb7)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*20*/DUN( "black_scar", 1, 1,	
	LEVS(
		LEV(MAPHI(4), CODE(21, 0x18c)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*21*/DUN( "mountain", 0, 2,		
	LEVS(
		LEV(MAPHI(5), CODE(9, 0x6c1)),
		LEV(MAPHI(6), CODE(5, 0x2ac)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*22*/DUN( "cyanis_tower", 0, 3,	
	LEVS(
		LEV(MAPHI(7), CODE(6, 0x140)),
		LEV(MAPHI(8), CODE(5, 0x13c)),
		LEV(MAPHI(9), CODE(7, 0x144)),
		LEV_NULL,
		LEV_NULL
		)
  ),
/*23*/DUN( "allirias_tomb", 0, 2,	
	LEVS(
		LEV(MAPHI(10), CODE(9, 0x4bc)),
		LEV(MAPHI(11), CODE(21, 0x2d4)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*24*/DUN( "wasteland", 0, 1,		
	LEVS(
		LEV(MAPHI(12), CODE(7, 0x40a)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*25*/DUN( "tarmitia", 0, 1,		
	LEVS(
		LEV(MAPHI(13), CODE(6, 0x325)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*26*/DUN( "berlin", 0, 1,		
	LEVS(
		LEV(MAPHI(14), CODE(9, 0x331)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*27*/DUN( "stalingrad", 0, 1,	
	LEVS(
		LEV(MAPHI(15), CODE(8, 0x32d)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*28*/DUN( "hiroshima", 0, 1,		
	LEVS(
		LEV(MAPHI(16), CODE(8, 0x32d)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*29*/DUN( "troy", 0, 1,		
	LEVS(
		LEV(MAPHI(17), CODE(8, 0x32d)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*30*/DUN( "rome", 0, 1,		
	LEVS(
		LEV(MAPHI(18), CODE(8, 0x32d)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*31*/DUN( "nottingham",0,  1,	
	LEVS(
		LEV(MAPHI(19), CODE(8, 0x32d)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*32*/DUN( "kun_wang", 0, 1,		
	LEVS(
		LEV(MAPHI(20), CODE(7, 0x421)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*33*/DUN( "malefia", 0, 3,		
	LEVS(
		LEV(MAPHI(21), CODE(10, 0x9ed)),
		LEV(MAPHI(22), CODE(8, 0x9e5)),
		LEV(MAPHI(23), CODE(15, 0xa01)),
		LEV_NULL,
		LEV_NULL
		)
  ),
/*34*/DUN( "barracks", 0, 1,		
	LEVS(
		LEV(MAPHI(24), CODE(3, 0x3d3)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*35*/DUN( "ferofist", 0, 1,		
	LEVS(
		LEV(MAPHI(25), CODE(11, 0x6c9)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*36*/DUN( "private_quarter", 0, 1,	
	LEVS(
		LEV(MAPHI(26), CODE(5, 0x358)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*37*/DUN( "workshop", 0, 1,		
	LEVS(
		LEV(MAPHI(27), CODE(3, 0x1d8)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*38*/DUN( "urmech_paradi", 0, 1,	
	LEVS(
		LEV(MAPHI(28), CODE(2, 0x4b0)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*39*/DUN( "viscous_plane", 0, 1,	
	LEVS(
		LEV(MAPHI(29), CODE(7, 0x2f6)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*40*/DUN( "sanctum", 0, 1,		
	LEVS(
		LEV(MAPHI(30), CODE(5, 0x3a0)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*41*/DUN( "tar_quarry", 0, 1,	
	LEVS(
		LEV(MAPHI(31), CODE(6, 0x406)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*42*/DUN( "shadow_canyon", 0, 1,	
	LEVS(
		LEV(MAPHI(32), CODE(6, 0x5ff)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*43*/DUN( "sceadus_demens", 0, 2,	
	LEVS(
		LEV(MAPHI(33), CODE(4, 0x4b8)),
		LEV(MAPHI(34), CODE(4, 0x4b8)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*44*/DUN( "tarjan", 0, 1,		
	LEVS(
		LEV(MAPHI(35), CODE(4, 0xf5)),
		LEV_NULL,
		LEV_NULL,
		LEV_NULL,
		LEV_NULL
		)
  ),
/*45*/DUN( 0,0,0,  
	LEVS( LEV_NULL, LEV_NULL, LEV_NULL, LEV_NULL, LEV_NULL )
  )
};

uint8_t dun_entryTypeList[] = {
  0, 0, 2, 0,
  0, 0, 0, 2,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  3, 3, 0, 0,
  0, 0, 0, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  0, 0, 5, 5,
  5, 6, 6, 6,
  6, 6, 6, 1,
  0, 0, 0, 0,
  2, 2, 1, 2
};

uint8_t dun_entrySqN[] = {
  0, 0, 4, 2,
  2, 2, 2, 9,
  0, 0, 0, 0,
  0, 0, 0, 0,
  4, 4, 4, 4,
  8, 8, 0, 0,
  0, 0, 0, 0,
  0, 6, 6, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0x0E, 0, 0x0A,
  0, 0, 0, 3,
  0, 0, 0, 0,
  0x10, 0x15, 1, 1,
  1, 0
};

uint8_t dun_entrySqE[] = {
  0, 0, 0x0E, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  4, 4, 4, 4,
  0, 0, 0, 0,
  2, 2, 1, 1,
  9, 9, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0x0A, 0x0A,
  0x0A, 0x0B, 0, 0,
  6, 6, 6, 0x0C,
  0x0E, 0x0E, 0x0E, 0x0E,
  0x0A, 3, 1, 1,
  0, 0
};

#define MONRANGE_LO(base, lo, hi)	\
	{ (((lo) - (base)) / 0x30), (((hi) - (base)) / 0x30) - 1 }

#define MONRANGE_HI(base, lo, hi)	\
	{ 				\
	  (((lo) - (base)) / 0x30) + ((0x2ed2 - 0x22) / 0x30), \
	  (((hi) - (base)) / 0x30) + (((0x2ed2 - 0x22) / 0x30) - 1)	\
	}
b3monIndex_t dun_monIndexList[] = {
  MONRANGE_LO(0x22, 0x0022, 0x0322),
  MONRANGE_LO(0x22, 0x0322, 0x0622),
  MONRANGE_LO(0x22, 0x0622, 0x0922),
  MONRANGE_LO(0x22, 0x0922, 0x0c22),
  MONRANGE_LO(0x22, 0x0c22, 0x0f82),
  MONRANGE_LO(0x22, 0x0f82, 0x12b2),
  MONRANGE_LO(0x22, 0x12b2, 0x15b2),
  MONRANGE_LO(0x22, 0x15b2, 0x18b2),
  MONRANGE_LO(0x22, 0x18b2, 0x1bb2),
  MONRANGE_LO(0x22, 0x1bb2, 0x1eb2),
  MONRANGE_LO(0x22, 0x1eb2, 0x21e2),
  MONRANGE_LO(0x22, 0x21e2, 0x24e2),
  MONRANGE_LO(0x22, 0x24e2, 0x26c2),
  MONRANGE_LO(0x22, 0x26c2, 0x28a2),
  MONRANGE_LO(0x22, 0x28a2, 0x2b12),
  MONRANGE_LO(0x22, 0x2b12, 0x2cf2),
  MONRANGE_LO(0x22, 0x2cf2, 0x2ed2),
  MONRANGE_HI(0x30, 0x0030, 0x0210),
  MONRANGE_HI(0x30, 0x0210, 0x0420),
  MONRANGE_HI(0x30, 0x0420, 0x0630),
  MONRANGE_HI(0x30, 0x0630, 0x0870),
  MONRANGE_HI(0x30, 0x0870, 0x0a80),
  MONRANGE_HI(0x30, 0x0a80, 0x0cc0),
  MONRANGE_HI(0x30, 0x0cc0, 0x0ff0),
  MONRANGE_HI(0x30, 0x0ff0, 0x12f0),
  MONRANGE_HI(0x30, 0x12f0, 0x15f0),
  MONRANGE_HI(0x30, 0x15f0, 0x1920),
  MONRANGE_HI(0x30, 0x1920, 0x1b30),
  MONRANGE_HI(0x30, 0x1b30, 0x1d40),
  MONRANGE_HI(0x30, 0x1d40, 0x1f50),
  MONRANGE_HI(0x30, 0x1f50, 0x2190),
  MONRANGE_HI(0x30, 0x2190, 0x2490),
  MONRANGE_HI(0x30, 0x2490, 0x2790),
  MONRANGE_HI(0x30, 0x2790, 0x2a90),
  MONRANGE_HI(0x30, 0x2a90, 0x2d90),
  MONRANGE_HI(0x30, 0x2d90, 0x2f70),
  MONRANGE_HI(0x30, 0x2f70, 0x3150),
  MONRANGE_HI(0x30, 0x3150, 0x3360),
  MONRANGE_HI(0x30, 0x3360, 0x36c0),
  MONRANGE_HI(0x30, 0x36c0, 0x3990),
  MONRANGE_HI(0x30, 0x3990, 0x3d50),
};

uint8_t dun_trapSave[] = {
  0x0F, 0x0F, 0x0E, 0,
  0x13, 0x13, 0x13, 0,
  0x15, 0x14, 0x14, 0,
  0x18, 0x18, 0x17, 0,
  0x26, 0x26, 0x28, 0,
  0x27, 0x28, 0x28, 0,
  0x2A, 0x29, 0x2A, 0,
  0x2B, 0x2E, 0x2B, 0
};

uint8_t dun_poisonDmg[] = {
  1, 2, 4, 8, 0x0A, 0x10, 0x14, 0x18
};

uint8_t dun_items[] = {
        0,
/*   */ 0x81, 0x81, 0x81, 0x81,
/*   */ 0x81, 0x81, 0x81, 0x81,
/*   */ 0x81, 0x81, 0x81, 0x81,
/*   */ 0x81, 0x81, 0x81, 0x81,
/*   */ 0x81, 0x81, 0x81, 0x81,
/*   */ 0x81, 0xC1, 0x41, 0x41,
/*   */ 0x41, 0x41, 0x41, 0x41,
/*   */ 0x41, 0x41, 0x41, 0x41,
/*   */ 0x41, 0x41, 0x41, 0x41,
/*   */ 0x41, 0x41, 0x41, 0x41,
/*   */ 0x41, 0x41, 0x41, 0x41,
/*   */ 0x41, 0x41, 0x31, 0x31,
/*   */ 0x31, 0x31, 0x31, 0x31,
/*   */ 0x31, 0x31, 0x31, 0x31,
/*   */ 0x31, 0x31, 0x31, 0x31,
/*   */ 0x31, 0x31, 0x31, 0x31,
/*   */ 0x31, 0x31, 0x31, 0x31,
/*   */ 0x31, 0x31, 0x31, 0x31,
/*   */ 0x19, 0x19, 0x19, 0x19,
/*   */ 0x19, 0x19, 0x19, 0x19,
/*   */ 0x19, 0x19, 0x19, 0x19,
/*   */ 0x19, 0x19, 0x19, 0x19,
/*   */ 0x19, 0x19, 0x19, 0x19,
/*   */ 0x19, 0x19, 0x19, 0x19,
/*   */ 0x19, 0x19, 0x19, 0x19,
/*   */ 0x19, 0x19, 0x19, 0x19,
/*   */ 0x19, 0x19, 0x19, 0x19,
/*   */ 0x19, 0x19, 0x19, 0,
/*   */ 0, 0, 0, 0,
/*   */ 0, 0xFF, 0, 0,
/*   */ 0, 0, 0, 0,
/*   */ 0xFF, 9, 9, 9,
/*   */ 9, 9, 9, 9,
/*   */ 9, 9, 0, 0,
/*   */ 0, 9, 9, 9,
/*   */ 9, 9, 9, 9,
/*   */ 0, 0, 0, 0,
/*   */ 0, 0, 0, 0,
/*   */ 0, 0, 0, 0,
/*   */ 0, 5, 5, 0,
/*   */ 0, 0, 0, 0,
/*   */ 9, 9, 9, 9,
/*   */ 0, 0, 3, 3,
/*   */ 0x41, 9, 0x11, 5,
/*   */ 1, 0, 0, 0,
/*   */ 9, 9, 9, 9,
/*   */ 9, 9, 9, 9,
/*   */ 9, 9, 9, 9,
/*   */ 9, 9, 0x7F, 5,
/*   */ 5, 5, 5, 5,
/*   */ 5, 5, 5, 5,
/*   */ 5, 5, 5, 5,
/*   */ 5, 5, 5, 5,
/*   */ 3, 3, 3, 3,
/*   */ 3, 3, 3, 3,
/*   */ 3, 3, 0x1D, 0,
/*   */ 0, 0, 0, 0,
/*   */ 0, 0, 0, 0,
/*   */ 0, 0, 0, 0,
/*   */ 0, 0, 0, 1,
/*   */ 1, 1, 1, 1,
/*   */ 1, 1, 1, 1,
/*   */ 0, 0, 0, 0,
/*   */ 0, 0, 0,
};

uint8_t dun_styleMap[] = {
  0, 1, 2, 2,
  2, 0, 3, 4,
  3, 3, 4, 4,
  4, 0, 3, 4
};

uint8_t dun_wallFlag[] = {
  0, 1, 0, 0,
  0, 1, 1, 0,
  0, 1, 0, 0,
  0, 1, 1, 0
};

