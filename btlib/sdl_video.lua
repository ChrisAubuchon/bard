require "bttable"

----------------------------------------
-- gfx class
--
-- Main window class
----------------------------------------

gfx = {
	screen		= nil,
	renderer	= nil,	
	texture		= nil,
	surface		= nil
}
function gfx:Init(inTitle)
	SDL.Init(SDL.INIT.TIMER, SDL.INIT.AUDIO, SDL.INIT.VIDEO)

	self.screen = SDL.CreateWindow(
			inTitle,
			SDL.WINDOWPOS.CENTERED,
			SDL.WINDOWPOS.CENTERED,
			640, 400, 0
			)
	self.renderer = SDL.CreateRenderer(gfx.screen, -1,
					SDL.RENDERER.SOFTWARE)
	self.texture = SDL.CreateTexture(gfx.renderer, 
			SDL.PIXELFORMAT.ABGR8888,
			SDL.TEXTUREACCESS.STREAMING,
			640,
			400)
	self.surface = SDL.CreateRGBSurface(640, 400, 32)
end

---------------------------------------------------------------------

----------------------------------------
-- gfxImage class
--
-- Handles graphics images and 
-- animations
----------------------------------------
gfxImage = {}
function gfxImage:new(inName, inType)
	if (inName == nil) then
		error("nil image name")
	end

	local self = object:new()

	self:addParent(gfxImage)

	self.surface	= false
	self.isAnim	= false

	if (inType == "png") then
		self.surface = SDL.LoadPNG(inName)
	elseif (inType == "bta") then
		self.surface = SDL.LoadBTA(inName)
		self.isAnim = true
	end

	return self
end


----------------------------------------
-- Draw()
----------------------------------------
function gfxImage:Draw(inDstRect)
	if (self.isAnim) then
		self.surface:start(gfx.renderer, gfx.texture, gfx.surface, 
					inDstRect)
	else
		gfx.surface:Blit(inDstRect, self.surface, nil)
		gfx.texture:Update(nil, gfx.surface)
		gfx.renderer:Copy(gfx.texture, nil, nil)
		gfx.renderer:Present()
	end
end

----------------------------------------
-- Clear()
----------------------------------------
function gfxImage:Clear()
	if (self.isAnim) then
		self.surface:stop()
	end
end

---------------------------------------------------------------------

----------------------------------------
-- gfxRect class
----------------------------------------
gfxRect = {}
function gfxRect:New(inX, inY, inW, inH)
	return SDL.Rect:New(inX, inY, inW, inH)
end

---------------------------------------------------------------------

----------------------------------------
-- gfxFont class
----------------------------------------
gfxFont = {}
function gfxFont:new(inName, inMonoFlag)
	local self = object:new()

	self:addParent(self, gfxFont)

	if (inMonoFlag) then
		self.font = SDL.font.Open(inName,
				SDL.font.FONT_BTF,
				SDL.font.BTF_MONOSPACE)
	else
		self.font = SDL.font.Open(inName,
				SDL.font.FONT_BTF,
				SDL.font.BTF_VARWIDTH)
	end

	return self
end

----------------------------------------
-- Size()
----------------------------------------
function gfxFont:Size(inText)
	return self.font:SizeText(inText)
end

----------------------------------------
-- Render()
----------------------------------------
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

---------------------------------------------------------------------

----------------------------------------
-- gfxSurface class
----------------------------------------
gfxSurface = {}
function gfxSurface:New(inWidth, inHeight)
	local self = object:new()

	self:addParent(gfxSurface)

	self.surface = false

	if ((inWidth) and (inHeight)) then
		self.surface = SDL.CreateRGBSurface(inWidth, inHeight, 32)
	end

	return self
end

----------------------------------------
-- Blit()
----------------------------------------
function gfxSurface:Blit(inDstRect, inSrc, inSrcRect)
	assert(inSrc.surface, "Trying to Blit a non-surface")

	self.surface:Blit(inDstRect, inSrc.surface, inSrcRect)
end

----------------------------------------
-- Fill()
----------------------------------------
function gfxSurface:Fill(inRectangle, inColor)
	self.surface:FillRect(inRectangle, inColor)
end

----------------------------------------
-- Draw()
----------------------------------------
function gfxSurface:Draw(inDstRect)
	gfx.surface:Blit(inDstRect, self.surface, nil)
	gfx.texture:Update(nil, gfx.surface)
	gfx.renderer:Copy(gfx.texture, nil, nil)
	gfx.renderer:Present()
end
