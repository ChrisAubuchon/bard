----------------------------------------
-- declare_global()
----------------------------------------
local function declare_global(name)
	rawset(_G, name, false)
end

declare_global("btroster")
declare_global("bICharacter")
declare_global("bIICharacter")

-- array.lua
declare_global("array")

-- bttable.lua
declare_global("btTable")
declare_global("btArray")
declare_global("btDefaultTable")

-- diskio.lua
declare_global("diskio")

-- entity.lua
declare_global("entity")

-- level.lua
declare_global("level")

-- linkedList.lua
declare_global("linkedList")
declare_global("linkedListNode")

-- object.lua
declare_global("object")

-- objectHash.lua
declare_global("objectHash")

-- random.lua
declare_global("random")

-- sdl_getkey.lua
declare_global("btkeys")
declare_global("getkey")

-- sdl_textbox.lua
declare_global("textBox")

-- sdl_timer.lua
declare_global("timer")

-- sdl_video.lua
declare_global("gfx")
declare_global("gfxImage")
declare_global("gfxRect")
declare_global("gfxFont")
declare_global("gfxSurface")
