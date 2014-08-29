bigpic = object:new()

bigpic.activeBigpic	= nil

bigpic.imgs	= diskio:readTable("bigpic")
bigpic.skara	= diskio:readTable("skaraview")
bigpic.wild	= diskio:readTable("wildview")
bigpic.gdung	= diskio:readTable("gdungview")
bigpic.titleRect	= gfxRect:New(32, 212, 224, 16)
bigpic.gfxRect	= gfxRect:New(32, 30, 224, 176)
bigpic.gdung.lightRect = {}
bigpic.gdung.lightRect[0] = gfxRect:New(0, 0, 224, 176)
bigpic.gdung.lightRect[1] = gfxRect:New(0, 40, 224, 96)
bigpic.gdung.lightRect[2] = gfxRect:New(0, 74, 224, 56)
bigpic.gdung.lightRect[3] = gfxRect:New(0, 88, 224, 30)
bigpic.gdung.lightRect[4] = gfxRect:New(0, 80, 224, 16)


bigpic.surface		= gfxSurface:New(224, 176)
bigpic.titleSurface	= gfxSurface:New(224, 16)

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
-- readImage()
----------------------------------------
function bigpic:readImage(inQuadName, inFacetName, inFacet, inBuilding)
	local format	= "images/%s/%s/%s-%s.png"
	local path

	inFacet.rect	= gfxRect:New(inFacet.x, inFacet.y,
					inFacet.w, inFacet.h)
	path		= string.format(format,
				currentLevel.tileSet,
				inQuadName,
				inBuilding,
				inFacetName
				)
	inFacet[inBuilding] = gfxImage:new(path, "png")
end

----------------------------------------
-- tileBackground
----------------------------------------
function bigpic:tileBackground()
	local tileSet	= currentLevel.tileSet

	if (globals.isNight) then
		if (not self[tileSet].nightbg) then
			self[tileSet].nightbg = gfxImage:new(
				"images/"..tileSet.."/bg-night.png", "png")
		end
		self.surface:Blit(nil, self[tileSet].nightbg)
	else
		if (not self[tileSet].daybg) then
			self[tileSet].daybg = gfxImage:new(
				"images/"..tileSet.."/bg-day.png", "png")
		end
		self.surface:Blit(nil, self[tileSet].daybg)
	end
end

----------------------------------------
-- tileRefresh()
----------------------------------------
function bigpic:tileRefresh()
	if (self.activeBigpic ~= nil) then
		self.activeBigpic:Clear()
		self.activeBigpic = nil
	end

	self.surface:Draw(self.gfxRect)
end

----------------------------------------
-- tileAdd()
----------------------------------------
function bigpic:tileAdd(inQuad, inDepth, inFacet, inBuilding)
	local quad
	local q
	local tileSet	= currentLevel.tileSet

	quad = string.format("%d%s", inDepth, inQuad)

	if (self[tileSet][quad][inFacet] == nil) then
		return
	end

	q = self[tileSet][quad][inFacet]
	if (not q[inBuilding]) then
		self:readImage(quad, inFacet, q, inBuilding)
	end

	self.surface:Blit(q.rect, q[inBuilding], nil)
end

if false then

----------------------------------------
-- dunBackground()
----------------------------------------
function bigpic:dunBackground(inDistance)
	if (not self.dun.daybg) then
		self.dun.daybg = gfxImage:new(
			"images/gdung/bg-day.png", "png")
	end
	self.surface:Blit(nil, self.dun.daybg, nil)
	if ((inDistance) and (inDistance < 5)) then
		self.surface:Fill(self.dun.lightRect[inDistance],
				globals.colors[1])
	end

if false then
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
		gfx = "portal"
	else
		if ((inSq.isSecret) and (party.light.seeSecret)) then
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



end





















