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
sys.dataPath = sys.dataPath .. "/bt2"
print(sys.dataPath)

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

getkey()

m_window:Draw(nil, gfxImage:new("images/screen.png", "png"), nil)
m_window:Update()

getkey()

require "icons"
require "bigpic"

if false then
local light = icons:new(icons.ICON_LIGHT)
local levitate = icons:new(icons.ICON_LEVITATE)
local shield = icons:new(icons.ICON_SHIELD)
local detect = icons:new(icons.ICON_DETECT)
--local compass = icons:new(icons.ICON_COMPASS)
light:activate()
levitate:activate()
shield:activate()
detect:activate()

getkey()
light:deactivate()
levitate:deactivate()
shield:deactivate()
detect:deactivate()
getkey()
end

bigpic:setBigpic("PIC_GUILDINT", "The Guild")
getkey()
