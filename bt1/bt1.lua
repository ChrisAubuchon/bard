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
require "globals"
require "btdebug"

video_init()

local w_title = sdl.img.LoadPNG("images/title.png")
m_window:Blit(nil, w_title, nil)
m_window:Flip()

require "bttable"
require "entity"
require "btlib"
require "btapi"
require "btdebug"
require "dataio"
require "battle"
require "action"
require "btio"
require "random"
require "text"
require "bt_text"
require "spells"
require "durationSpell"
require "icons"
require "race"
require "classes"
require "level"
require "city"
require "dun"
require "garths"
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
require "timer"
require "songs"
require "monster"
require "summon"
require "noncombat"
require "api-monster"
require "api-dun"
require "api-battle"
require "api-city"

getkey()

w_title = sdl.img.LoadPNG("images/screen.png")
m_window:Blit(nil, w_title, nil)
m_window:Flip()

repeat
	if (globals.gameState == globals.STATE_GUILD) then
		globals.partyDied = false
		local xxx_bard_song_timeout
		party.detect.deactivate()
		party.shield.deactivate()
		party.levitate.deactivate()
		party.light.deactivate()
		party.compass.deactivate()
		guild.enter()
	elseif (globals.gameState == globals.STATE_CITY) then
		currentLevel.main()
	elseif (globals.gameState == globals.STATE_DUNGEON) then
		currentLevel.main()
	elseif (globals.gameState == globals.STATE_PARTYDIED) then
		globals.gameState = globals.STATE_GUILD
	end
until (globals.gameState == globals.STATE_EXIT)
