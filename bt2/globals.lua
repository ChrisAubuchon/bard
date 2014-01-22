local colors = {}
globals = {}

colors[1] = 	{   0,   0,   0	}
colors[2] = 	{   0,   0, 170	}
colors[3] = 	{   0, 170,   0	}
colors[4] = 	{   0, 170, 170	}
colors[5] = 	{ 170,   0,   0	}
colors[6] = 	{ 170,   0, 170	}
colors[7] = 	{ 170,  85,   0	}
colors[8] = 	{ 170, 170, 170	}
colors[9] = 	{  85,  85,  85	}
colors[10] = 	{  85,  85, 255	}
colors[11] = 	{  85, 255,  85	}
colors[12] = 	{  85, 255, 255	}
colors[13] = 	{ 255,  85,  85	}
colors[14] = 	{ 255,  85, 255	}
colors[15] = 	{ 255, 255,  85	}
colors[16] = 	{ 255, 255, 255	}

globals.doTimeEvents	= false
globals.gameHour	= 7
globals.inDungeon	= false
globals.isNight		= false
globals.isPaused	= false
globals.colors		= colors
globals.debug		= true
globals.partyDied	= false
globals.citySquare	= false
globals.guildCity	= "Tangramayne"

--m_window		= gfxScreen:new()
globals.fonts		= {}
globals.fonts.mono	= gfxFont:new("fonts/bt2font.btf", true)
globals.fonts.var	= gfxFont:new("fonts/bt2font.btf", false)
text			= textBox:new(
				gfxRect:New(340, 12, 276, 192),
				22,
				globals.fonts.var
				)
garths			= false

currentLevel		= false
currentBattle		= false

directions = {
	front = {
		north	= "north",
		south	= "south",
		east	= "east",
		west	= "west"
	},
	back = {
		north	= "south",
		south	= "north",
		east	= "west",
		west	= "east"
	},
	left = {
		north	= "west",
		south	= "east",
		east	= "north",
		west	= "south"
	},
	right = {
		north	= "east",
		south	= "west",
		east	= "south",
		west	= "north"
	}
}

globals.STATE_GUILD	= 1
globals.STATE_INGAME	= 2
globals.STATE_PARTYDIED	= 3
globals.gameState	= globals.STATE_GUILD

globals.timeOfDay = {
	"midnight.",
	"midnight.",
	"after midnight.",
	"after midnight.",
	"after midnight.",
	"after midnight.",
	"early morning.",
	"early morning.",
	"early morning.",
	"mid-morning.",
	"mid-morning.",
	"noon.",
	"noon.",
	"noon.",
	"afternoon.",
	"afternoon.",
	"afternoon.",
	"dusk.",
	"dusk.",
	"evening.",
	"evening.",
	"evening.",
	"evening.",
	"evening.",
	"midnight."
}

stringTables = {}
stringTables.effects = {
	kill	= ", killing ",
	poison	= " and poisons ",
	drain	= " and drains a level from",
	nuts	= " and inflicts insanity on ",
	old	= " and withers ",
	possess = " and possesses ",
	stone	= " and stones ",
	critical= " and critically hits "
}

stringTables.andEffects = {
	fry	= "fries",
	freeze	= "froze",
	shock	= "shocks",
	drain	= "drains",
	burn	= "burns",
	choke	= "chokes",
	steam	= "steams"
}

stringTables.isEffects = {
	fry	= "fried",
	freeze	= "frozen",
	shock	= "shocked",
	drain	= "drained",
	burn	= "burnt",
	choke	= "choked",
	steam	= "steamed"
}

