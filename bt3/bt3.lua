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

currentLevel = {}

local title
title = gfxImage:new("images/tit0.png", "png")
title:Draw(nil)

getkey()

require "bigpic"

local title
title = gfxImage:new("images/main_screen.png", "png")
title:Draw(nil)

bigpic:setBigpic("PIC_H5", "H5")
getkey()

currentLevel.tileSet = "skara"
bigpic:tileBackground()
bigpic:tileAdd("M", 2, "face", "0")
bigpic:tileAdd("L", 2, "face", "1")
bigpic:tileAdd("R", 2, "face", "3")
bigpic:tileRefresh()
getkey()

bigpic:tileBackground()
bigpic:tileAdd("L", 3, "side", "1")
bigpic:tileAdd("R", 3, "side", "0")
bigpic:tileAdd("L", 2, "side", "1")
bigpic:tileAdd("R", 2, "side", "0")
bigpic:tileAdd("L", 1, "side", "2")
bigpic:tileAdd("R", 1, "side", "3")
bigpic:tileRefresh()
getkey()

bigpic:tileBackground()
bigpic:tileAdd("L", 3, "face", "1")
bigpic:tileAdd("M", 3, "face", "0")
bigpic:tileAdd("R", 3, "face", "2")
bigpic:tileRefresh()
getkey()

bigpic:tileBackground()
bigpic:tileAdd("FL", 4, "face", "0")
bigpic:tileAdd("L", 4, "face", "1")
bigpic:tileAdd("M", 4, "face", "2")
bigpic:tileAdd("R", 4, "face", "3")
bigpic:tileAdd("FR", 4, "face", "0")
bigpic:tileRefresh()
getkey()

currentLevel.tileSet = "wild"

bigpic:tileBackground()
bigpic:tileAdd("M", 2, "face", "0")
bigpic:tileAdd("L", 2, "face", "1")
bigpic:tileAdd("R", 2, "face", "2")
bigpic:tileRefresh()
getkey()

bigpic:tileBackground()
bigpic:tileAdd("M", 3, "face", "0")
bigpic:tileAdd("L", 3, "face", "1")
bigpic:tileAdd("R", 3, "face", "1")
bigpic:tileAdd("M", 2, "face", "2")
bigpic:tileAdd("L", 2, "face", "3")
bigpic:tileAdd("R", 2, "face", "4")
bigpic:tileRefresh()
getkey()

if false then

currentLevel.tileSet = "gdung"

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

end

os.exit(0)

