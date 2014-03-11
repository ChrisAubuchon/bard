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
globals.guildCity	= false
globals.inSnare		= false

globals.fonts		= {}
globals.fonts.mono	= gfxFont:new("fonts/bt3font.btf", true)
globals.fonts.var	= gfxFont:new("fonts/bt3font.btf", false)
if false then
text			= textBox:new(
				gfxRect:New(340, 12, 276, 192),
				22,
				globals.fonts.var
				)
end
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
globals.STATE_EXIT	= 3
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
	steam	= "steams",
	blast	= "blasts",
	hit	= "hits"
}

stringTables.isEffects = {
	fry	= "fried",
	freeze	= "frozen",
	shock	= "shocked",
	drain	= "drained",
	burn	= "burnt",
	choke	= "choked",
	steam	= "steamed",
	blast	= "blasted",
	hit	= "hit"
}


----------------------------------------
-- globals:saveGame()
----------------------------------------
function globals:saveGame()
	local saveState		= {}

	text:cdprint(true, false, "Do you wish to save your game?")
	if (not text:getYesNo()) then
		return
	end

	text:cdprint(true, false, "Saving the game...")

	saveState.currentLevel	= currentLevel:saveState()
	saveState.party		= party:saveState()
	saveState.gameHour	= globals.gameHour
	saveState.garth		= table.copy(garths.inventory)

	diskio:writeTable(saveState, "savedGame")

	timer:delay(1)

	text:cdprint(true, false, "Your game has been saved to disk.\n\n" ..
				 "Do you wish to exit?")
	if (text:getYesNo()) then
		os.exit(0)
	end

	text:clear()
end

----------------------------------------
-- globals:restoreGame()
----------------------------------------
function globals:restoreGame()
	local savedState	= diskio:readTable("savedGame")

	globals.gameHour	= savedState.gameHour

	if (savedState.currentLevel.class == "city") then
		currentLevel = city:restoreState(savedState.currentLevel)
	elseif (savedState.currentLevel.class == "dun") then
		currentLevel = dun:restoreState(savedState.currentLevel)
	elseif (savedState.currentLevel.class == "wild") then
		currentLevel = wild:restoreState(savedState.currentLevel)
	else
		error("Invalid currentLevel.class", 2)
	end

	garths.inventory	= table.copy(savedState.garth)

	party:restoreState(savedState.party)
	party.compass:update(currentLevel.direction)
end
