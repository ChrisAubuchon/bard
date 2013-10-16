require "bttable"

local vid_conf = {
	main_window = {
		width = 640,
		height = 400,
		depth = 8
	},
	title = "Bard's Tale Engine"
}

----------------------------------------
-- gfxScreen()
--
-- Main screen.  Really just a gfxWindow
-- with a different surface initializer
----------------------------------------
gfxScreen = {}
function gfxScreen:new()
	local self = gfxWindow:new()

	sdl.Init(sdl.SDL_INIT_TIMER, sdl.SDL_INIT_AUDIO, sdl.SDL_INIT_VIDEO)

	self.surface = sdl.SetVideoMode(vid_conf.main_window.width,
					vid_conf.main_window.height,
					vid_conf.main_window.depth,
					sdl.SDL_HWSURFACE, sdl.SDL_HWPALETTE,
					sdl.SDL_DOUBLEBUF)

	return self
end

----------------------------------------
-- gfxWindow()
--
-- Higher level graphics functions
----------------------------------------
gfxWindow = {}
function gfxWindow:new(w,h,d)
	local self = {
		surface	= false
	}

	if (w ~= nil) then
		self.surface = sdl.CreateRGBSurface(w,h,d)
	end

	btTable.addParent(self, gfxWindow)
	btTable.setClassMetatable(self)

	return self
end

----------------------------------------
-- Fill()
--
-- Fill a rectangle with a color
----------------------------------------
function gfxWindow:Fill(inRectangle, inColor)
	self.surface:FillRect(inRectangle, inColor)
end

----------------------------------------
-- Update()
--
-- Update a rectangle. If no rectangle
-- is passed, update the entire surface
----------------------------------------
function gfxWindow:Update(inRectangle)
	if (inRectangle == nil) then
		self.surface:Flip()
	else
		self.surface:UpdateRect(inRectangle)
	end
end

----------------------------------------
-- Draw()
--
-- Draw one gfxWindow in another
-- gfxWindow
----------------------------------------
function gfxWindow:Draw(inDestRect, inWindow, inSrcRect)
	if (inWindow == nil) then
		error("Draw(): Nil window argument", 2)
	end

	self.surface:Blit(inDestRect, inWindow.surface, inSrcRect)
end

----------------------------------------
-- SetColor()
--
-- Change the palette of the gfxWindow
----------------------------------------
function gfxWindow:SetColor(inIndex, inColor)
	self.surface:SetColor(inIndex, inColor)
end

----------------------------------------
-- gfxImage()
--
-- A surface with functions for handling
-- animations
----------------------------------------
gfxImage = {}
function gfxImage:new(inName, inType)
	local self = {
		surface	= false,
		isAnim	= false
	}

	if (inType == "png") then
		if (inName == nil) then
			error("NIL image name", 2)
		end
		self.surface = sdl.img.LoadPNG(inName)
	elseif (inType == "bta") then
		self.surface = sdl.img.LoadBTA(inName)
	else
		error("Invalid image type: " .. tostring(inType), 2)
	end

	self.isAnim = self.surface.isAnim

	btTable.addParent(self, gfxImage)
	btTable.setClassMetatable(self)

	return self
end

----------------------------------------
-- Draw()
--
-- Draw an image on a gfxWindow.
-- If self is an animation, start the
-- animation sequence
----------------------------------------
function gfxImage:Draw(inParent, inDestRect)
	if (self.surface.isAnim) then
		self.surface:start(inParent.surface, inDestRect)
	else
		inParent:Draw(inDestRect, self, nil)
		inParent:Update(inDestRect)
	end
end

----------------------------------------
-- Clear()
--
-- Remove the image. This currently
-- just stops an animation.
----------------------------------------
function gfxImage:Clear()
	if (self.surface.isAnim) then
		self.surface:stop()
	end
end

----------------------------------------
-- gfxRectangle()
----------------------------------------
gfxRectangle = {}
function gfxRectangle:new(x,y,w,h)
	if (x == nil) then
		error("NIL x value", 2)
	end
	return sdl.NewRect(x,y,w,h)
end

----------------------------------------
-- gfxColor()
----------------------------------------
gfxColor = {}
function gfxColor:new(r,g,b)
	return sdl.NewColor(r,g,b)
end

----------------------------------------
-- gfxFont class
----------------------------------------

gfxFont = {}
function gfxFont:new(inName, inMono)
	local self = {
		font	= false
	}

	if (inMono) then
		self.font = sdl.font.Open(inName,
				sdl.font.FONT_BTF,
				sdl.font.BTF_MONOSPACE)
	else
		self.font = sdl.font.Open(inName,
				sdl.font.FONT_BTF,
				sdl.font.BTF_VARWIDTH)
	end

	btTable.addParent(self, gfxFont)
	btTable.setClassMetatable(self)

	return self
end

function gfxFont:Size(inText)
	return self.font:SizeText(inText)
end

function gfxFont:Render(inText, inColor)
	local rval = {
		surface = false,
		w	= 0,
		h	= 0
	}

	rval.surface = self.font:RenderText(inText, inColor)
	rval.w = rval.surface.w
	rval.h = rval.surface.h
	return rval
end

----------------------------------------
-- getkey()
--
-- Main IO loop
----------------------------------------
function getkey()
	local inkey

	while true do
		inkey = sdl.GetKey()

		if (not inkey) then
			return nil
		elseif (inkey == sdl.SDLK_BACKSPACE) then
			return btkeys.BTKEY_BACKSPACE
		elseif (inkey == sdl.SDLK_RETURN) then
			return btkeys.BTKEY_RETURN
		elseif (inkey == sdl.SDLK_ESCAPE) then
			return btkeys.BTKEY_ESCAPE
		elseif (inkey == sdl.SDLK_UP) then
			return btkeys.BTKEY_UP
		elseif (inkey == sdl.SDLK_DOWN) then
			return btkeys.BTKEY_DOWN
		elseif (inkey == sdl.SDLK_LEFT) then
			return btkeys.BTKEY_LEFT
		elseif (inkey == sdl.SDLK_RIGHT) then
			return btkeys.BTKEY_RIGHT
		elseif (inkey == sdl.SDLK_PAGEUP) then
			return btkeys.BTKEY_PAGEUP
		elseif (inkey == sdl.SDLK_PAGEDOWN) then
			return btkeys.BTKEY_PAGEDOWN
		elseif (inkey == sdl.SDLK_F1) then
			return btkeys.BTKEY_F1
		elseif (inkey == sdl.SDLK_F2) then
			return btkeys.BTKEY_F2
		elseif (inkey == sdl.SDLK_F3) then
			return btkeys.BTKEY_F3
		elseif (inkey == sdl.SDLK_F4) then
			return btkeys.BTKEY_F4
		elseif (inkey == sdl.SDLK_F5) then
			return btkeys.BTKEY_F5
		elseif (inkey == sdl.SDLK_F6) then
			return btkeys.BTKEY_F6
		elseif (inkey == sdl.SDLK_F7) then
			return btkeys.BTKEY_F7
		elseif (inkey == 0) then
			return inkey
		elseif (inkey == btkeys.BTKEY_RANDOMBATTLE) then
			return btkeys.BTKEY_RANDOMBATTLE
		elseif (inkey == btkeys.BTKEY_WANDERING) then
			dprint("Wandering monster")
		elseif (inkey < 255) then
			return string.upper(string.char(inkey))
		end
	end
end

