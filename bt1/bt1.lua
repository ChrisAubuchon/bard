----------------------------------------
-- declare_global()
--
-- Global variables need to be declared
-- before they can be used. This is to
-- easily catch typos in code. If you
-- try to assign a value to a variable
-- that has not been declared the game
-- will error
----------------------------------------
function declare_global(name, initval)
	rawset(_G, name, initval or false)
end

----------------------------------------
-- Set up the global registry to only
-- index and newindex on a previously
-- declared value
----------------------------------------

-- Set the data path
--
package.path = package.path .. ";" .. sys.dataPath .. "/../btlib/?.lua"
sys.sharedPath = sys.dataPath .. "/share"
sys.dataPath = sys.dataPath .. "/bt1"

setmetatable(_G, {
	__newindex = function(_, n)
		error("Attempt to write to undeclared global " .. n, 2)
	end,
	__index = function(_, n)
		error("Attempt to read from undeclared global " .. n, 2)
	end,
	})


require "declares"
require "btlib"

gfx:Init("Bard's Tale")
require "globals"
require "btdebug"

local title = gfxImage:new("images/title.png", "png")
title:Draw(nil)

require "btdebug"
require "b1lib"
require "bigpic"
require "battleBonus"
require "battle"
require "action"
require "spells"
require "icons"
require "durationSpell"
require "race"
require "classes"
require "building"
require "cityBuildings"
require "cityGuardian"
require "cityIrongate"
require "city"
require "dunSquares"
require "dun"
require "chest"
require "shoppe"
require "tavern"
require "roscoes"
require "temple"
require "review"
require "bigpic"
require "character"
require "guild"
require "roster"
require "party"
require "items"
require "inventory"
require "timers"
require "songs"
require "monster"
require "monsterGroup"
require "monsterParty"
require "summon"

getkey()

title = gfxImage:new("images/screen.png", "png")
title:Draw(nil)

repeat
	if (globals.gameState == globals.STATE_GUILD) then
		globals.partyDied = false
		if (party.song.singer) then
			party.song.singer:songTimeout()
		end
		party.detect:deactivate()
		party.shield:deactivate()
		party.levitate:deactivate()
		party.light:deactivate()
		party.compass:deactivate()
		guild.enter()
	elseif (globals.gameState == globals.STATE_CITY) then
		currentLevel:main()
	elseif (globals.gameState == globals.STATE_DUNGEON) then
		currentLevel:main()
	elseif (globals.gameState == globals.STATE_PARTYDIED) then
		globals.gameState = globals.STATE_GUILD
	end
until (globals.gameState == globals.STATE_EXIT)
