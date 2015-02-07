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
package.path = package.path .. ";../btlib/?.lua"
sys.sharedPath = "../data/share"
sys.dataPath   = "../data/bt3"

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

log.level = log.LOG_DEBUG

gfx:Init("Bard's Tale III")

require "globals"

local title
title = gfxImage:new("images/tit0.png", "png")
title:Draw(nil)

getkey()

require "bigpic"
require "race"
require "classes"
require "roster"
require "party"
require "camp"
require "itemList"
require "character"

title = gfxImage:new("images/main_screen.png", "png")
title:Draw(nil)

repeat
	if (globals.gameState == globals.STATE_GUILD) then
		camp:enter()
	elseif (globals.gameState == globals.STATE_INGAME) then
		currentLevel:main()
	elseif (globals.gameState == globals.STATE_DIED) then
		party:died()
	end
until (globals.gameState == globals.STATE_EXIT)
