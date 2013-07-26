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
require "sdl_video"
require "sdl_timer"
require "sdl_text"
require "globals"
require "btdebug"

m_window:Draw(nil, gfxImage:new("images/title.png", "png"), nil)
m_window:Update()

require "bttable"
require "linkedList"
require "entity"
require "btapi"
require "btdebug"
require "dataio"
require "battleBonus"
require "battle"
require "action"
require "random"
require "spells"
require "icons"
require "durationSpell"
require "race"
require "classes"
require "level"
require "city"
require "dun"
require "shoppe"
require "tavern"
require "roscoes"
require "temple"
require "review"
require "citylib"
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
require "api-monster"
require "api-dun"
require "api-battle"
require "api-city"

getkey()

m_window:Draw(nil, gfxImage:new("images/screen.png", "png"), nil)
m_window:Update()

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
