local __bigpic = {}
function __bigpic:new()
	local self = {
		titleRect	= false,
		gfxRect		= false,
		activeBigpic	= nil,
		surface		= false,
		imgs		= false,
		city		= {},
		dun		= {},
		wild		= {}
	}

	btTable.addParent(self, __bigpic)
	btTable.setClassMetatable(self)

	self.imgs	= diskio:readTable("bigpic")
	self.city	= diskio:readTable("cityview")
	self.wild	= diskio:readTable("wildview")
	self.dun	= diskio:readTable("dunview")
	self.titleRect	= gfxRectangle:new(32, 212, 224, 16)
	self.gfxRect	= gfxRectangle:new(32, 30, 224, 176)
	self.dun.lightRect = {}
	self.dun.lightRect[0] = gfxRectangle:new(0, 0, 224, 176)
	self.dun.lightRect[1] = gfxRectangle:new(0, 10, 224, 156)
	self.dun.lightRect[2] = gfxRectangle:new(0, 26, 224, 108)
	self.dun.lightRect[3] = gfxRectangle:new(0, 48, 224, 66)
	self.dun.bg = {}
	self.dun.bg[0] = gfxImage:new("images/dpics/0-bg.png", "png")
	self.dun.bg[1] = gfxImage:new("images/dpics/1-bg.png", "png")
	self.dun.bg[2] = gfxImage:new("images/dpics/2-bg.png", "png")

	self.surface = gfxWindow:new(224, 176, 8)

	return self
end

bigpic = __bigpic:new()

----------------------------------------
-- setBigpic()
--
-- Helper function to set the image and
-- title in one shot
----------------------------------------
function bigpic:setBigpic(inName, inTitle)
	self:drawImage(inName)
	self:setTitle(inTitle)
end

----------------------------------------
-- setTitle()
----------------------------------------
function bigpic:setTitle(format, ...)
	local title
	local fontP
	local surface
	local w
	local h
	local x

	if (format == nil) then
		error("setTitle() with nil format", 2)
	end

	title = string.format(format, ...)
	fontP = globals.fonts.var

	w,h = fontP:Size(title)
	if (w > 224) then
		return
	end

	surface = fontP:Render(title, globals.colors[16])
	x = ((224 - w) / 2) + 36

	m_window:Fill(self.titleRect, globals.colors[1])
	m_window:Draw(gfxRectangle:new(x,212,0,0), surface, nil)
	m_window:Update(self.titleRect)
end

----------------------------------------
-- drawImage()
----------------------------------------
function bigpic:drawImage(inName, inIsTimeAware)
	local isTimeAware = inIsTimeAware or false

	-- Dynamically create the graphics image if it
	-- has not already been created
	--
	if (not self.imgs[inName].img) then
		local v = self.imgs[inName]
		v.img = gfxImage:new(v.path, v.type)
	end

	if (self.activeBigpic ~= nil) then
		self.activeBigpic:Clear()
		self.activeBigpic = nil
	end

	self.activeBigpic = self.imgs[inName].img
	if (globals.isNight and isTimeAware) then
		self.surface:Draw(nil, self.activeBigpic, nil)
		self.surface:Update()
		self.surface:SetColor(11, globals.colors[1])
		m_window:Draw(self.gfxRect, self.surface, nil)
		m_window:Update(self.gfxRect)
		self.surface:SetColor(11, globals.colors[12])
	else
		self.activeBigpic:Draw(m_window, self.gfxRect)
	end
end

----------------------------------------
-- readCityImage()
----------------------------------------
function bigpic:readCityImage(inQuadName, inFacetName, inFacet, inBuilding)
	local format = "images/citypics/%s/%s/%s.png"
	local path

	if (type(self.imgs[inBuilding]) == "table") then
		local v = self.imgs[inBuilding]
		v.img = gfxImage:new(v.path, v.type)
		return
	end

	inFacet.rect = gfxRectangle:new(inFacet.x, inFacet.y,
					inFacet.w, inFacet.h)
	path = string.format(format, inQuadName, inFacetName, inBuilding)
	inFacet[inBuilding] = gfxImage:new(path, "png")
end

----------------------------------------
-- cityBackground()
----------------------------------------
function bigpic:cityBackground()
	if (not self.city.bg) then
		self.city.bg = gfxImage:new("images/citypics/citybg.png", "png")
	end
	self.surface:Draw(nil, self.city.bg, nil)
end

----------------------------------------
-- cityAdd
----------------------------------------
function bigpic:cityAdd(inCycle, inDepth, inFacet, inBuilding)
	local quad
	local q

	quad = inCycle .. "-" .. inDepth

	if (self.city[quad][inFacet] == nil) then
		return
	end

	q = self.city[quad][inFacet]
	if (not q[inBuilding]) then
		self:readCityImage(quad, inFacet, q, inBuilding)
	end

	if (q[inBuilding] == nil) then
		self.surface:Draw(nil, self.imgs[inBuilding].img, nil)
	else
		self.surface:Draw(q.rect, q[inBuilding], nil)
	end
end

----------------------------------------
-- cityDisplay()
----------------------------------------
function bigpic:cityDisplay()
	if (self.activeBigpic ~= nil) then
		self.activeBigpic:Clear()
		self.activeBigpic = nil
	end

	self.surface:Update()
	if (globals.isNight) then
		self.surface:SetColor(11, globals.colors[1])
	end
	m_window:Draw(self.gfxRect, self.surface, nil)
	m_window:Update()

	if (globals.isNight) then
		self.surface:SetColor(11, globals.colors[12])
	end
end

----------------------------------------
-- wildBackground()
----------------------------------------
function bigpic:wildBackground()
	if (not self.wild.bg) then
		self.wild.bg = gfxImage:new("images/wpics/wpic.png", "png")
	end
	self.surface:Draw(nil, self.wild.bg, nil)
end

----------------------------------------
-- bigpic:readWildImage()
----------------------------------------
function bigpic:readWildImage(inFacetName, inFacet, inBuilding)
	local format	= "images/wpics/%s/%s.png"
	local path

	inFacet.rect = gfxRectangle:new(inFacet.x, inFacet.y, 
					inFacet.w, inFacet.h)
	path = string.format(format, inFacetName, inBuilding)
	inFacet.gfx = gfxImage:new(path, "png")
end

----------------------------------------
-- bigpic:wildAdd()
----------------------------------------
function bigpic:wildAdd(inDepth, inFacet, inBuilding)
	local facet
	local q

	facet = inDepth .. "-" .. inFacet

	if (self.wild[facet][inBuilding] == nil) then
		return
	end

	q = self.wild[facet][inBuilding]
	if (not q.gfx) then
		self:readWildImage(facet, q, inBuilding)
	end
	self.surface:Draw(q.rect, q.gfx, nil)
end

----------------------------------------
-- wildDisplay()
----------------------------------------
function bigpic:wildDisplay()
	self:cityDisplay()
end

----------------------------------------
-- dunBackground()
----------------------------------------
function bigpic:dunBackground(inTileSet)
	if (not party.light.active) then
		self.surface:Fill(self.dun.lightRect[0], globals.colors[1])
		return
	end

	self.surface:Draw(nil, self.dun.bg[inTileSet], nil)
	if (party.light.distance < 4) then
		self.surface:Fill(self.dun.lightRect[party.light.distance],
				globals.colors[1])
	end
end

----------------------------------------
-- bigpic:readDunImage()
----------------------------------------
function bigpic:readDunImage(inQuadName, inFacetName, inGfx, inTileSet)
	local format	= "images/dpics/%s/%s/%d-%s.png"
	local path

	path = string.format(format, inQuadName, inFacetName, inTileSet, inGfx)
	return gfxImage:new(path, "png")
end

----------------------------------------
-- dunAdd()
----------------------------------------
function bigpic:dunAdd(inQuad, inTileSet, inFacet, inSq)
	local gfx
	local q

	if (self.dun[inQuad][inFacet] == nil) then
		return	
	end

	if (inFacet == "portal") then
		if (inSq.hasCeilPortal) then
			gfx = "ceil"
		end
		if (inSq.hasFloorPortal) then
			gfx = "floor"
		end
	else
		if ((inSq.secret) and (party.light.seeSecret)) then
			gfx = "door"
		else
			gfx = inSq.gfx
		end
	end

	q = self.dun[inQuad][inFacet]
	if (not q.rect) then
		q.rect = gfxRectangle:new(q.x, q.y, q.w, q.h)
	end

	if (not q[gfx]) then
		q[gfx] = {}
	end

	if (not q[gfx][inTileSet]) then
		q[gfx][inTileSet] = self:readDunImage(inQuad, inFacet, 
						      gfx, inTileSet)
	end
	
	self.surface:Draw(q.rect, q[gfx][inTileSet], nil)
end

function bigpic:dunDisplay()
	if (self.activeBigpic ~= nil) then
		self.activeBigpic:Clear()
		self.activeBigpic = nil
	end

	self.surface:Update()
	m_window:Draw(self.gfxRect, self.surface, nil)
	m_window:Update(self.gfxRect)
end
























