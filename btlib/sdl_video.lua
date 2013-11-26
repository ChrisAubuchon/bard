require "test/bttable"

local m_screen = nil
m_renderer = nil
m_texture = nil
m_surface = nil

function gfxInit(inTitle)
	SDL.Init(SDL.INIT.TIMER, SDL.INIT.AUDIO, SDL.INIT.VIDEO)

	m_screen = SDL.CreateWindow(
			inTitle,
			SDL.WINDOWPOS.CENTERED,
			SDL.WINDOWPOS.CENTERED,
			640, 400, 0
			)
	m_renderer = SDL.CreateRenderer(m_screen, -1, SDL.RENDERER.SOFTWARE)
	m_texture = SDL.CreateTexture(m_renderer, 
			SDL.PIXELFORMAT.ABGR8888,
			SDL.TEXTUREACCESS.STREAMING,
			640,
			400)
	m_surface = SDL.CreateRGBSurface(640, 400, 32)
end

----------------------------------------
-- gfxImage class
--
-- Handles graphics images and 
-- animations
----------------------------------------
gfxImage = {}
function gfxImage:new(inName, inType)
	local self = {
		surface	= false,
		isAnim	= false
	}

	if (inName == nil) then
		error("nil image name")
	end

	if (inType == "png") then
		self.surface = SDL.LoadPNG(inName)
	elseif (inType == "bta") then
		self.surface = SDL.LoadBTA(inName)
		self.isAnim = true
	end

	btTable.addParent(self, gfxImage)
	btTable.setClassMetatable(self)

	return self
end


----------------------------------------
-- Draw()
----------------------------------------
function gfxImage:Draw(inDstRect)
	if (self.isAnim) then
		self.surface:start(m_renderer, m_texture,m_surface, inDstRect)
	else
		m_surface:Blit(inDstRect, self.surface, nil)
		m_texture:Update(nil, m_surface)
		m_renderer:Copy(m_texture, nil, nil)
		m_renderer:Present()
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

----------------------------------------
-- gfxRect class
----------------------------------------
gfxRect = {}
function gfxRect:New(inX, inY, inW, inH)
	return SDL.Rect:New(inX, inY, inW, inH)
end


btkeys = {
	RETURN = ":A:",
	ESCAPE = ":B:",
	BACKSPACE = ":C:",
	UP = ":D:",
	DOWN = ":E:",
	LEFT = ":F:",
	RIGHT = ":G:",
	PAGEUP = ":H:",
	PAGEDOWN = ":I:",
	F1 = ":J:",
	F2 = ":K:",
	F3 = ":L:",
	F4 = ":M:",
	F5 = ":N:",
	F6 = ":O:",
	F7 = ":P:",
	RANDOMBATTLE = ":Q:",
	WANDERING = ":R:"
}

----------------------------------------
-- getkey()
--
-- Main IO loop
----------------------------------------
function getkey()
	local inkey

	while true do
		inkey = SDL.GetKey()

		if (not inkey) then
			return nil
		elseif (inkey == 0) then
			return inkey
		elseif (inkey == SDL.K.RETURN) then
			return btkeys.RETURN
		elseif (inkey == SDL.K.BACKSPACE) then
			return btkeys.BACKSPACE
		elseif (inkey == SDL.K.ESCAPE) then
			return btkeys.ESCAPE
		elseif (inkey == SDL.K.UP) then
			return btkeys.UP
		elseif (inkey == SDL.K.DOWN) then
			return btkeys.DOWN
		elseif (inkey == SDL.K.LEFT) then
			return btkeys.LEFT
		elseif (inkey == SDL.K.RIGHT) then
			return btkeys.RIGHT
		elseif (inkey == SDL.K.PAGEUP) then
			return btkeys.PAGEUP
		elseif (inkey == SDL.K.PAGEDOWN) then
			return btkeys.PAGEDOWN
		elseif (inkey == SDL.K.F1) then
			return btkeys.F1
		elseif (inkey == SDL.K.F2) then
			return btkeys.F2
		elseif (inkey == SDL.K.F3) then
			return btkeys.F3
		elseif (inkey == SDL.K.F4) then
			return btkeys.F4
		elseif (inkey == SDL.K.F5) then
			return btkeys.F5
		elseif (inkey == SDL.K.F6) then
			return btkeys.F6
		elseif (inkey == SDL.K.F7) then
			return btkeys.F7
		elseif (inkey < 255) then
			return string.upper(string.char(inkey))
		end
	end
end

