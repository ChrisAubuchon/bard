#include <b2lib.h>
#include <level.h>

uint8_t *locationTitleList[] = {
	"Wilderness",
	"Tangramayne",
	"Ephesus",
	"Philippi",
	"Colosse",
	"Corinth",
	"Thessalonica",
	"Dark Domain",
	"The Tombs",
	"The Castle",
	"The Tower",
	"Maze of Dread",
	"Oscon",
	"Grey Crypt",
	"Destiny Stone"
};

uint8_t poisonDamageList[] = {
	0, 1, 2, 4, 6, 8, 10, 16, 20
};

uint8_t rewardItemBase[] = {
	0, 8, 8, 0x10, 0x20, 0x30, 0x40, 0x51
};

uint8_t rewardItemRange[] = {
	0x0f, 0x0f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f 
};

uint8_t	cityStartX[] = {
	0xff, 0, 4, 7, 0, 6, 0x0f, 0
};

uint8_t cityStartY[] = {
	0xff, 7, 0, 0x0f, 9, 0x0f, 7, 0
};

uint8_t cityToWildX[] = {
	0xff, 2, 15, 15, 21, 4, 26
};

uint8_t cityToWildY[] = {
	0xff, 24, 5, 34, 15, 43, 40
};

uint8_t wildToCityX[] = {
	0xff, 0, 4, 7, 0, 6, 15, 0
};

uint8_t wildToCityY[] = {
	0xff, 8, 15, 0, 6, 0, 8
};

uint8_t *citySpecialFace[] = {
	NULL,
	"PIC_BAREXT",
	"PIC_TEMPLEEXT",
	"PIC_GUILDEXT",
	"PIC_REVEXT",
	"PIC_ROSCOEEXT",
	NULL,
	NULL,
	"PIC_GARTHEXT",
	NULL,
	NULL,
	NULL,
	"PIC_TEMPLEEXT",
	"PIC_IRONGATE",
	NULL,
	NULL
};

uint8_t *cityStreetName[] = {
	"Alley", "Main", "Traveller", "Scimitar", "Claymore",
	"Denizen", "Battleaxe", "Lowshield", "Scepter", "spunspell",
	"Bluesteel", "Ybarra", "Brazen", "Bogster", "Corwin",
	"Gordon", "Flyblade", "Shortspell", "Mycroft", "Masterstroke",
	"Jade Fist", "Jabberwock", "Knollglen", "Crossblade", "Riposte",
	"Twister", "Masonry", "Craggy", "Lower", "Stoney",
	"Crafty", "Balk", "Far Archer", "Apollo", "Odin",
	"Mithril", "Mesmerizer", "Tapestry", "Trickster", "Malarky",
	"Crescent", "Zender", "Bein", "Marakesh"
};

uint8_t	*cityEvent[] = {
	"cityBuildings:enter(\"empty\")",
	"tavern:new",
	"temple:new",
	"cityBuildings:enter(\"guild\")",
	"review:enter()",
	"roscoes:enter()",
	"cityBuildings:enter(\"gate\")",
	"--xxx-invalid-code-7",
	"garths:enter()",
	"cityBuildings:enter(\"casino\")",
	"bank:enter()",
	"--xxx-invalid-code-11",
	"--dungeon-C",
	"--xxx-invalid-code-12",
	"--dungeon-E",
	"--dungeon-F"
};

uint8_t *cityDungeon[] = {
	NULL,
	"cityBuildings:enter(\"domain\")",
	"cityBuildings:enter(\"tombs\")",
	"cityBuildings:enter(\"tower\")",
	"cityBuildings:enter(\"stone\")",
	"cityBuildings:enter(\"fort\")",
	"cityBuildings:enter(\"maze\")",
};

int8_t cityTavernY[] = {
	0, 7,15, 3, 8,10,14, 2, 5, 5,13, 6, 8,11,13,13, 1, 3,14,15,-1
};

int8_t cityTavernX[] = {
	7,11, 8, 5, 2,13, 8, 2, 5,13, 4, 2,15, 3, 9,13,13,13, 9,13,-1
};

uint8_t	*cityTavernName[] = {
	"Zandurae Inn", "Blacksmoke", "Foul Breath", "Zinskea Inn",
	"Bein Tavern", "Skull Tavern", "Graysword Inn", "Warmonger Inn",
	"Ganzu Inn", "Redpoint Inn", "Millenium Inn", "Fun Run Inn",
	"Orc-skinner", "Blue buckler", "Crimson Bard", "Lazy Dragon",
	"Zen Tavern", "Bone Breaker", "Crescent Kick", "Viod Epee Inn",
	"Tavern"
};


int8_t cityTempleY[] = {
	1, 2,13, 6, 6,14, 3, 5, 7, 7, 2, 3, 7, 9,11,13, 3, 9, 9, 0,-1
};

int8_t cityTempleX[] = {
	0,13, 6, 2,12,12, 8, 8, 2,13, 8,12,12, 3,10,13, 3, 0, 5, 0,-1
};

uint8_t *cityTempleName[] = {
	"Vroom Temple", "Fixer Temple", "Brotherly Love", "Temple of Rok",
	"One God", "The I AM", "101 Blessings", "102 Blessings",
	"The Mad God", "Shadow Temple", "Bogus Shrine", "14 Miracles",
	"Cool Cleric", "Funky Friar", "Limbo Time", "Party Shrine",
	"15 Miracles", "Lone Disciple", "Manic Monk", "Temple"
};

uint8_t guildExitSqN[] = {
	0, 7, 2, 13, 8, 13, 8, 0
};

uint8_t guildExitSqE[] = {
	0, 1, 8, 7, 8, 2, 10, 0
};

uint8_t *guildExitDir[] = {
	NULL,
	"west",
	"south",
	"south",
	"west",
	"west",
	"south",
	NULL
};
