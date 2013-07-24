require "dataio"

local colors = {}
globals = {}

colors[1] = gfxColor:new(0,0,0)
colors[2] = gfxColor:new(0,0,170)
colors[3] = gfxColor:new(0,170,0)
colors[4] = gfxColor:new(0,170,170)
colors[5] = gfxColor:new(170,0,0)
colors[6] = gfxColor:new(170,0,170)
colors[7] = gfxColor:new(170,85,0)
colors[8] = gfxColor:new(170,170,170)
colors[9] = gfxColor:new(85,85,85)
colors[10] = gfxColor:new(85,85,255)
colors[11] = gfxColor:new(85,255,85)
colors[12] = gfxColor:new(85,255,255)
colors[13] = gfxColor:new(255,85,85)
colors[14] = gfxColor:new(255,85,255)
colors[15] = gfxColor:new(255,255,85)
colors[16] = gfxColor:new(255,255,255)

globals.doTimeEvents	= false
globals.gameHour	= 7
globals.inDungeon	= false
globals.isNight		= false
globals.isPaused	= false
globals.colors		= colors
globals.debug		= true
globals.partyDied	= false
globals.swapWallsAndDoors	= false

m_window		= gfxScreen:new()
globals.fonts		= {}
globals.fonts.mono	= gfxFont:new("fonts/bt1font.btf", true)
globals.fonts.var	= gfxFont:new("fonts/bt1font.btf", false)
text			= textBox:new(m_window)
garths			= false

currentLevel		= false
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
globals.STATE_CITY	= 2
globals.STATE_DUNGEON	= 3
globals.STATE_EXIT	= 4
globals.STATE_PARTYDIED	= 5
globals.gameState	= globals.STATE_GUILD

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
	

btkeys = {
	BTKEY_RETURN = ":A:",
	BTKEY_ESCAPE = ":B:",
	BTKEY_BACKSPACE = ":C:",
	BTKEY_UP = ":D:",
	BTKEY_DOWN = ":E:",
	BTKEY_LEFT = ":F:",
	BTKEY_RIGHT = ":G:",
	BTKEY_PAGEUP = ":H:",
	BTKEY_PAGEDOWN = ":I:",
	BTKEY_F1 = ":J:",
	BTKEY_F2 = ":K:",
	BTKEY_F3 = ":L:",
	BTKEY_F4 = ":M:",
	BTKEY_F5 = ":N:",
	BTKEY_F6 = ":O:",
	BTKEY_F7 = ":P:",
	BTKEY_RANDOMBATTLE	= ":Q:",
	BTKEY_WANDERING		= ":R:"
}

