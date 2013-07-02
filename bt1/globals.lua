require "dataio"

local fonts = {}
local colors = {}
globals = {}

colors[1] = video_color(0,0,0)
colors[2] = video_color(0,0,170)
colors[3] = video_color(0,170,0)
colors[4] = video_color(0,170,170)
colors[5] = video_color(170,0,0)
colors[6] = video_color(170,0,170)
colors[7] = video_color(170,85,0)
colors[8] = video_color(170,170,170)
colors[9] = video_color(85,85,85)
colors[10] = video_color(85,85,255)
colors[11] = video_color(85,255,85)
colors[12] = video_color(85,255,255)
colors[13] = video_color(255,85,85)
colors[14] = video_color(255,85,255)
colors[15] = video_color(255,255,85)
colors[16] = video_color(255,255,255)

globals.doTimeEvents	= false
globals.gameHour	= 7
globals.inDungeon	= false
globals.isNight		= false
globals.isPaused	= false
globals.fonts		= fonts
globals.colors		= colors
globals.debug		= true
globals.partyDied	= false

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
