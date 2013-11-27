local colors = {}
globals = {}

colors[1] = 	{    0,   0,   0, 255 }
colors[2] = 	{    0,   0, 170, 255 }
colors[3] = 	{    0, 170,   0, 255 }
colors[4] = 	{    0, 170, 170, 255 }
colors[5] = 	{  170,   0,   0, 255 }
colors[6] = 	{  170,   0, 170, 255 }
colors[7] = 	{  170,  85,   0, 255 }
colors[8] = 	{  170, 170, 170, 255 }
colors[9] = 	{   85,  85,  85, 255 }
colors[10] = 	{   85,  85, 255, 255 }
colors[11] = 	{   85, 255,  85, 255 }
colors[12] = 	{   85, 255, 255, 255 }
colors[13] = 	{  255,  85,  85, 255 }
colors[14] = 	{  255,  85, 255, 255 }
colors[15] = 	{  255, 255,  85, 255 }
colors[16] = 	{  255, 255, 255, 255 }

globals.doTimeEvents	= false
globals.gameHour	= 7
globals.inDungeon	= false
globals.isNight		= false
globals.isPaused	= false
globals.colors		= colors
globals.debug		= true
globals.partyDied	= false
globals.citySquare	= false
globals.swapWallsAndDoors	= false

--m_window		= gfxScreen:new()
globals.fonts		= {}
globals.fonts.mono	= gfxFont:new("fonts/bt1font.btf", true)
globals.fonts.var	= gfxFont:new("fonts/bt1font.btf", false)
text			= textBox:new(
				gfxRect:New(352, 34, 262, 192), 
				21,
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

