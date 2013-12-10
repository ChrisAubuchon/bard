local __bigpic = {}
function __bigpic:new()
	local self = {
		titleRect	= false,
		titleSurface	= false,
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
	self.titleRect	= gfxRect:New(32, 212, 224, 16)
	self.gfxRect	= gfxRect:New(32, 30, 224, 176)
	self.dun.lightRect = {}
	self.dun.lightRect[0] = gfxRect:New(0, 0, 224, 176)
	self.dun.lightRect[1] = gfxRect:New(0, 10, 224, 156)
	self.dun.lightRect[2] = gfxRect:New(0, 26, 224, 108)
	self.dun.lightRect[3] = gfxRect:New(0, 48, 224, 66)
	self.dun.bg = {}
	self.dun.bg[0] = gfxImage:new("images/dpics/0-bg.png", "png")
	self.dun.bg[1] = gfxImage:new("images/dpics/1-bg.png", "png")
	self.dun.bg[2] = gfxImage:new("images/dpics/2-bg.png", "png")

	self.surface		= gfxSurface:New(224, 176)
	self.titleSurface	= gfxSurface:New(224, 16)

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
	x = (224 - w) / 2

	self.titleSurface:Fill(nil, globals.colors[1]);
	self.titleSurface:Blit(gfxRect:New(x,0,0,0), surface, nil)
	self.titleSurface:Draw(self.titleRect)
end

----------------------------------------
-- drawImage()
----------------------------------------
function bigpic:drawImage(inName)
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
	if (self.imgs[inName].isTimeAware) then
		if (globals.isNight) then
			self.surface:Fill(nil, globals.colors[1])
		else
			self.surface:Fill(nil, globals.colors[12])
		end
		self.surface:Draw(self.gfxRect)
	end
	self.activeBigpic:Draw(self.gfxRect)
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

	inFacet.rect = gfxRect:New(inFacet.x, inFacet.y,
					inFacet.w, inFacet.h)
	path = string.format(format, inQuadName, inFacetName, inBuilding)
	inFacet[inBuilding] = gfxImage:new(path, "png")
end

----------------------------------------
-- cityBackground()
----------------------------------------
function bigpic:cityBackground()
	if (globals.isNight) then
		if (not self.city.nightbg) then
			self.city.nightbg = gfxImage:new(
				"images/citypics/citybg-night.png", "png")
		end
		self.surface:Blit(nil, self.city.nightbg)
	else
		if (not self.city.daybg) then
			self.city.daybg = gfxImage:new(
				"images/citypics/citybg-day.png", "png")
		end
		self.surface:Blit(nil, self.city.daybg)
	end
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
		self.surface:Blit(nil, self.imgs[inBuilding].img, nil)
	else
		self.surface:Blit(q.rect, q[inBuilding], nil)
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

	self.surface:Draw(self.gfxRect)
end

----------------------------------------
-- wildBackground()
----------------------------------------
function bigpic:wildBackground()
	if (globals.isNight) then	
		if (not self.wild.nightbg) then
			self.wild.nightbg = gfxImage:new(
				"images/wpics/wpic-night.png", "png")
		end
		self.surface:Blit(nil, self.wild.nightbg)
	else
		if (not self.wild.daybg) then
			self.wild.daybg = gfxImage:new(
				"images/wpics/wpic-day.png", "png")
		end
		self.surface:Blit(nil, self.wild.daybg)
	end
end

----------------------------------------
-- bigpic:readWildImage()
----------------------------------------
function bigpic:readWildImage(inFacetName, inFacet, inBuilding)
	local format	= "images/wpics/%s/%s.png"
	local path

	inFacet.rect = gfxRect:New(inFacet.x, inFacet.y, 
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
	self.surface:Blit(q.rect, q.gfx, nil)
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

	self.surface:Blit(nil, self.dun.bg[inTileSet], nil)
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

	if ((inFacet == "floor") or (inFacet == "ceiling")) then
		gfx = inFacet
	else
		if ((inSq.secret) and (party.light.seeSecret)) then
			gfx = "door"
		else
			gfx = inSq.gfx
		end
	end

	q = self.dun[inQuad][inFacet]
	if (not q.rect) then
		q.rect = gfxRect:New(q.x, q.y, q.w, q.h)
	end

	if (not q[gfx]) then
		q[gfx] = {}
	end

	if (not q[gfx][inTileSet]) then
		q[gfx][inTileSet] = self:readDunImage(inQuad, inFacet, 
						      gfx, inTileSet)
	end
	
	self.surface:Blit(q.rect, q[gfx][inTileSet], nil)
end

function bigpic:dunDisplay()
	if (self.activeBigpic ~= nil) then
		self.activeBigpic:Clear()
		self.activeBigpic = nil
	end

	self.surface:Draw(self.gfxRect)
end
























