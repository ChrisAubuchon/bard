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
		dun		= {}
	}

	local function readImages()
		local k
		local v

		self.imgs = diskio:readTable("bigpic")
		for k,v in pairs(self.imgs) do
			v.img = gfxImage:new(v.path, v.type)
		end
	end

	local function makeRectangles()
		self.titleRect	= gfxRect:New(32, 212, 224, 16)
		self.gfxRect	= gfxRect:New(32, 30, 224, 176)
	end

	local function readCityImages()
		local quad
		local quadname
		local facet
		local facetname
		local buildingString = "images/citypics/%s/%s/building%d.png"

		local function __readImages(f, fn, qn)
			local i
			local nameFormat = "building%d"

			f.rect = gfxRect:New(f.x, f.y, f.w, f.h)
			for i = 1,4 do
				local index = string.format(nameFormat, i)
				local path=string.format(buildingString,qn,fn,i)

				f[index] = gfxImage:new(path, "png")
			end
		end
		

		self.city = diskio:readTable("cityview")
		for quadname,quad in pairs(self.city) do
			for facetname,facet in pairs(quad) do
				__readImages(facet, facetname, quadname)
			end
		end

		self.city.daybg = gfxImage:new("images/citypics/citybg-day.png", "png")
		self.city.nightbg = gfxImage:new("images/citypics/citybg-night.png", "png")
	end

	local function readDunImages()
		local quad
		local quadname
		local facet
		local facetname

		self.dun = diskio:readTable("dunview")
		for quadname, quad in pairs(self.dun) do
			for facetname, facet in pairs(quad) do
				local path

				path = "images/dpics/"..quadname.."/"..facetname
				facet.rect = gfxRect:New(
					facet.x, facet.y, facet.w, facet.h)

				if (facetname == "floor") then
					facet.floor = {}
					facet.floor[0] = gfxImage:new(
						path.."/0-portal.png", "png")
					facet.floor[1] = gfxImage:new(
						path.."/1-portal.png", "png")
					facet.floor[2] = gfxImage:new(
						path.."/2-portal.png", "png")
				elseif (facetname == "ceiling") then
					facet.ceiling = {}
					facet.ceiling[0] = gfxImage:new(
						path.."/0-portal.png", "png")
					facet.ceiling[1] = gfxImage:new(
						path.."/1-portal.png", "png")
					facet.ceiling[2] = gfxImage:new(
						path.."/2-portal.png", "png")
				else
					facet.door = {}
					facet.door[0] = gfxImage:new(
						path.."/0-door.png", "png")
					facet.door[1] = gfxImage:new(
						path.."/1-door.png", "png")
					facet.door[2] = gfxImage:new(
						path.."/2-door.png", "png")
					facet.wall = {}
					facet.wall[0] = gfxImage:new(
						path.."/0-wall.png", "png")
					facet.wall[1] = gfxImage:new(
						path.."/1-wall.png", "png")
					facet.wall[2] = gfxImage:new(
						path.."/2-wall.png", "png")
				end
			end
		end
		self.dun.bg = {}
		self.dun.bg[0] = gfxImage:new("images/dpics/0-bg.png", "png")
		self.dun.bg[1] = gfxImage:new("images/dpics/1-bg.png", "png")
		self.dun.bg[2] = gfxImage:new("images/dpics/2-bg.png", "png")

		self.dun.lightRect = {}
		self.dun.lightRect[0] = gfxRect:New(0, 0, 224, 176)
		self.dun.lightRect[1] = gfxRect:New(0, 10, 224, 156)
		self.dun.lightRect[2] = gfxRect:New(0, 26, 224, 108)
		self.dun.lightRect[3] = gfxRect:New(0, 48, 224, 66)
	end


	btTable.addParent(self, __bigpic)
	btTable.setClassMetatable(self)

	readImages()
	makeRectangles()
	readCityImages()
	readDunImages()

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
	x = ((224 - w) / 2) 

	self.titleSurface:Fill(nil, globals.colors[1])
	self.titleSurface:Blit(gfxRect:New(x,0,0,0), surface, nil)
	self.titleSurface:Draw(self.titleRect)
end

----------------------------------------
-- drawImage()
----------------------------------------
function bigpic:drawImage(inName)
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
-- cityBackground()
----------------------------------------
function bigpic:cityBackground()
	if (globals.isNight) then
		self.surface:Blit(nil, self.city.nightbg)
	else
		self.surface:Blit(nil, self.city.daybg)
	end
end

----------------------------------------
-- cityAdd()
----------------------------------------
function bigpic:cityAdd(inQuad, inFacet, inBuilding)
	local q

	if (self.city[inQuad][inFacet] == nil) then
		return
	end

	q = self.city[inQuad][inFacet]

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
		if ((inSq.isSecret) and (party.light.seeSecret)) then
			gfx = "door"
		else
			gfx = inSq.gfx
		end

		if (globals.swapWallsAndDoors) then
			if (gfx == "door") then
				gfx = "wall"
			else
				gfx = "door"
			end
		end
	end

	q = self.dun[inQuad][inFacet]
	self.surface:Blit(q.rect, q[gfx][inTileSet], nil)
end

----------------------------------------
-- dunDisplay()
----------------------------------------
function bigpic:dunDisplay()
	if (self.activeBigpic ~= nil) then
		self.activeBigpic:Clear()
		self.activeBigpic = nil
	end

	self.surface:Draw(self.gfxRect)
end























