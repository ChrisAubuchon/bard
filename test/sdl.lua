sdl = {}

sdl.SDLK_BACKSPACE	= 512
sdl.SDLK_RETURN		= 513
sdl.SDLK_ESCAPE		= 514
sdl.SDLK_UP		= 515
sdl.SDLK_DOWN		= 516
sdl.SDLK_LEFT		= 517
sdl.SDLK_RIGHT		= 518
sdl.SDLK_PAGEUP		= 519
sdl.SDLK_PAGEDOWN	= 520
sdl.SDLK_F1		= 521
sdl.SDLK_F2		= 522
sdl.SDLK_F3		= 523
sdl.SDLK_F4		= 524
sdl.SDLK_F5		= 525
sdl.SDLK_F6		= 526
sdl.SDLK_F7		= 527

sdl.img = {}

function sdl.img.LoadPNG(name)
	local rval = SDLSurface:new(1,2,3)
	rval.isAnim = false
	return rval
end

function sdl.img.LoadBTA(name)
	local rval = SDLSurface:new(1,2,3)
	rval.isAnim = true
	return rval
end

function sdl.CreateRGBSurface(w,h,d)
	return SDLSurface:new(w,h,d)
end

function sdl.Init(...)
end

function sdl.SetVideoMode(w,h,d, ...)
	return SDLSurface:new(w,h,d)
end

function sdl.NewRect(x,y,w,h)
	return SDLRect:new(x,y,w,h)
end

function sdl.NewColor(r,g,b)
	return SDLColor:new(r,g,b)
end

local timerCount = 0
function sdl.AddTimer(inFunction, interval)
	dprint("AddTimer() called")
	timerCount = timerCount + 1
	return timerCount
end

function sdl.Sleep(inTime)
	dprint("Sleep(%d) called", inTime)
	timerCount = timerCount + 1
	return timerCount
end

function sdl.RemoveTimer(inTimer)
	dprint("RemoveTimer(%d) called", inTimer)
end

function sdl.NewTextbox(parent, x,y,w,h)
	return SDLTextBox:new(x,y,w,h)
end

SDLTextBox = {}
function SDLTextBox:new(x,y,w,h)
	local self = {
		x	= x,
		y	= y,
		w	= w,
		h	= h,
		curx	= 0,
		cury	= 0,
		bg_color	= false,
		fg_color	= false,
		hg_color	= false
	}

	local mt = {}
	mt.__index = SDLTextBox
	setmetatable(self, mt)
	self.__index = self

	return self
end

function SDLTextBox:Draw()
	dprint("SDLTextBox:Draw()")
end

function SDLTextBox:Clear()
	dprint("SDLTextBox:Clear()")
end

function SDLTextBox:SetCursor(x,y)
	self.curx = x
	self.cury = y
	dprint("SetCursor(%d,%d)", x, y)
end

function SDLTextBox:GetCursor(x,y)
	return self.curx,self.cury
end

function SDLTextBox:SetFont(inFont)
	dprint("SetFont(%s)", tostring(inFont))
end

function SDLTextBox:Print(inString)
	dprint("Print(\"%s\")", inString)
end

function SDLTextBox:Highlight(inLineno)
	dprint("Highlight(%d)", inLineno)
end

function SDLTextBox:UnHighlight(inLineno)
	dprint("UnHighlight(%d)", inLineno)
end

function SDLTextBox:putc(inC)
	dprint("putc(%d)", inC)

	return 1
end

function SDLTextBox:eraseChar(...)
	dprint("eraseChar()")
end

function SDLTextBox:clearLine()
	dprint("clearLine()")
end

SDLColor = {}
function SDLColor:new(r,g,b)
	local self = {
		r	= r,
		g	= g,
		b	= b
	}

	local mt = {}
	mt.__tostring = function (r)
		return string.format("color(%d,%d,%d)", r.r,r.g,r.b)
	end
	mt.__index = SDLColor
	setmetatable(self, mt)
	self.__index = self

	return self
end

SDLRect = {}
function SDLRect:new(x,y,w,h)
	local self = {
		x	= x,
		y	= y,
		w	= w,
		h	= h
	}

	local mt = {}
	mt.__tostring = function (r) 
		return string.format("rect(%d,%d,%d,%d)", r.x, r.y, r.w, r.h) 
		end
	mt.__index = SDLRect
	setmetatable(self, mt)

	self.__index = self

	return self
end


SDLSurface = {}
function SDLSurface:new(w,h,d)
	local self = {
		w	= w,
		h	= h,
		d	= d,
		pitch	= 0,
		isAnim	= false
	}

	local mt = {}
	mt.__tostring = function (r)
		return string.format("surface(%d,%d,%d,%d,%s)", r.w,r.h,r.d,r.pitch,r.isAnim)
		end
	mt.__index = SDLSurface
	setmetatable(self, mt)
	self.__index = self

	return self
end

function SDLSurface:UpdateRect(x,y,w,h)
	if (type(x) == "table") then
		dprint("UpdateRect(%s,%s) called", tostring(self), tostring(x))
	else
		dprint("UpdateRect(%s,%d,%d,%d,%d)", tostring(self), x, y, w, h)
	end
end

function SDLSurface:FillRect(...)
	local index = 1
	local val
	local outString = "FillRect("

	val = select(index, ...)
	if (type(val) == "table") then
		outString = outString .. tostring(val)
		index = index + 1
	else
		local x
		local y
		local w
		local h

		x,y,w,h = select(index, ...)
		outString = outString .. string.format("%d,%d,%d,%d",x,y,w,h)
		index = index + 4
	end

	outString = outString .. ", "

	val = select(index, ...)
	if (type(val) == "table") then
		outString = outString .. tostring(val)
	else
		local r
		local g
		local b

		r,g,b = select(index, ...)
		outString = outString .. string.format("%d,%d,%d",r,g,b)
	end

	outString = outString .. ")"

	dprint(outString)
end

function SDLSurface:Blit(...)
	dprint("Blit() called")
end

function SDLSurface:Flip(...)
	dprint("Flip() called")
end

function SDLSurface:SetColor(...)
	dprint("SetColor() called")
end

sdl.font = {}
sdl.font.FONT_BTF	= 0x1000
sdl.font.FONT_TTF	= 0x2000
sdl.font.BTF_MONOSPACE	= 0x4000
sdl.font.BTF_VARWIDTH	= 0x8000

function sdl.font.Open(n,t,f)
	return SDLFont:new(n,t,f)
end

function sdl.font.Close(inFont)
	dprint("sdl.font.Close() called")
end

SDLFont = {}
function SDLFont:new(n,t,f)
	local self = {
		name		= n,
		type		= t,
		flags		= f
	}

	local mt = {}
	mt.__tostring = function (f) return f.name end
	mt.__index = SDLFont
	setmetatable(self, mt)
	self.__index = self

	return self
end

function SDLFont:RenderText(t, c)
	dprint("RenderText(\"%s\")", t)
	return SDLSurface:new(0,0,100,100)
end

function SDLFont:SizeText(t, c)
	return 1,1
end

local testKeys = {}
function sdl.inputKeys(inKeys)
	testKeys = inKeys
end

function sdl.GetKey()
	local rval = table.remove(testKeys, 1)
	if (type(rval) == "number") then
		return rval
	end

	if (rval == nil) then
		return 0
	else
		return string.byte(rval)
	end
end
