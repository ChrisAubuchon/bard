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

require "bigpic"

local title
title = gfxImage:new("images/main_screen.png", "png")
title:Draw(nil)

bigpic:cityBackground()
bigpic:cityAdd("M", 2, "face", "0")
bigpic:cityAdd("L", 2, "face", "1")
bigpic:cityAdd("R", 2, "face", "3")
bigpic:cityDisplay()
getkey()

bigpic:cityBackground()
bigpic:cityAdd("L", 3, "side", "1")
bigpic:cityAdd("R", 3, "side", "0")
bigpic:cityAdd("L", 2, "side", "1")
bigpic:cityAdd("R", 2, "side", "0")
bigpic:cityAdd("L", 1, "side", "2")
bigpic:cityAdd("R", 1, "side", "3")
bigpic:cityDisplay()
getkey()

bigpic:cityBackground()
bigpic:cityAdd("L", 3, "face", "1")
bigpic:cityAdd("M", 3, "face", "0")
bigpic:cityAdd("R", 3, "face", "2")
bigpic:cityDisplay()
getkey()

bigpic:cityBackground()
bigpic:cityAdd("FL", 4, "face", "0")
bigpic:cityAdd("L", 4, "face", "1")
bigpic:cityAdd("M", 4, "face", "2")
bigpic:cityAdd("R", 4, "face", "3")
bigpic:cityAdd("FR", 4, "face", "0")
bigpic:cityDisplay()
getkey()

bigpic:wildBackground()
bigpic:wildAdd("M", 2, "face", "0")
bigpic:wildAdd("L", 2, "face", "1")
bigpic:wildAdd("R", 2, "face", "2")
bigpic:wildDisplay()
getkey()

bigpic:wildBackground()
bigpic:wildAdd("M", 3, "face", "0")
bigpic:wildAdd("L", 3, "face", "1")
bigpic:wildAdd("R", 3, "face", "1")
bigpic:wildAdd("M", 2, "face", "2")
bigpic:wildAdd("L", 2, "face", "3")
bigpic:wildAdd("R", 2, "face", "4")
bigpic:wildDisplay()
getkey()

bigpic:dunBackground()
bigpic:dunDisplay()
getkey()
bigpic:dunBackground(1)
bigpic:dunDisplay()
getkey()
bigpic:dunBackground(2)
bigpic:dunDisplay()
getkey()
bigpic:dunBackground(3)
bigpic:dunDisplay()
getkey()
bigpic:dunBackground(4)
bigpic:dunDisplay()
getkey()
bigpic:dunBackground(5)
bigpic:dunDisplay()
getkey()

os.exit(0)
