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
sys.dataPath = sys.dataPath .. "/bt2"

setmetatable(_G, {
	__newindex = function(_, n)
		error("Attempt to write to undeclared global " .. n, 2)
	end,
	__index = function(_, n)
		error("Attempt to read from undeclared global " .. n, 2)
	end,
	})


require "declares"
require "btlib_declares"
require "diskio"
require "sdl_video"
require "sdl_timer"
require "sdl_text"
require "globals"
require "btdebug"

m_window:Draw(nil, gfxImage:new("images/title.png", "png"), nil)
m_window:Update()

getkey()

m_window:Draw(nil, gfxImage:new("images/screen.png", "png"), nil)
m_window:Update()

require "icons"
require "bigpic"
require "random"
require "btapi"
require "linkedList"
require "btroster"
require "roster"
require "battleBonus"
require "battleMonster"
require "battlePlayer"
require "battleParty"
require "character"
require "monster"
require "monsterParty"
require "monsterGroup"
require "durationSpell"
require "party"
require "action"
require "guild"
require "level"
require "building"
require "sagehut"
require "wild"
require "bank"
require "temple"
require "tavern"
require "review"
require "roscoes"
require "shoppe"
require "cityBuildings"
require "city"
require "dunSquares"
require "dun"
require "battle"
require "songs"

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
		guild:enter()
	elseif (globals.gameState == globals.STATE_INGAME) then
		currentLevel:main()
	end
until (globals.gameState == globals.STATE_EXIT)

