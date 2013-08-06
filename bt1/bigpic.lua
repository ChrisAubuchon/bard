local __bigpic = {}
function __bigpic:new()
	local self = {
		titleRect	= false,
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

		self.imgs = read_table("bigpic")
		for k,v in pairs(self.imgs) do
			v.img = gfxImage:new(v.path, v.type)
		end
	end

	local function makeRectangles()
		self.titleRect	= gfxRectangle:new(32, 212, 224, 16)
		self.gfxRect	= gfxRectangle:new(32, 30, 224, 176)
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

			f.rect = gfxRectangle:new(f.x, f.y, f.x, f.h)
			for i = 1,4 do
				local index = string.format(nameFormat, i)
				local path=string.format(buildingString,qn,fn,i)

				f[index] = gfxImage:new(path, "png")
			end
		end
		

		self.city = read_table("cityview")
		for quadname,quad in pairs(self.city) do
			for facetname,facet in pairs(quad) do
				__readImages(facet, facetname, quadname)
			end
		end

		self.city.bg = gfxImage:new("images/citypics/citybg.png", "png")
	end

	local function readDunImages()
		local quad
		local quadname
		local facet
		local facetname

		self.dun = read_table("dunview")
		for quadname, quad in pairs(self.dun) do
			for facetname, facet in pairs(quad) do
				local path

				path = "images/dpics/"..quadname.."/"..facetname
				facet.rect = gfxRectangle:new(
					facet.x, facet.y, facet.w, facet.h)

				if (facetname == "portal") then
					facet.floor = {}
					facet.floor[0] = gfxImage:new(
						path.."/0-floor.png", "png")
					facet.floor[1] = gfxImage:new(
						path.."/1-floor.png", "png")
					facet.floor[2] = gfxImage:new(
						path.."/2-floor.png", "png")
					facet.ceiling = {}
					facet.ceiling[0] = gfxImage:new(
						path.."/0-ceiling.png", "png")
					facet.ceiling[1] = gfxImage:new(
						path.."/1-ceiling.png", "png")
					facet.ceiling[2] = gfxImage:new(
						path.."/2-ceiling.png", "png")
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
		self.dun.lightRect[0] = gfxRectangle:new(0, 0, 224, 176)
		self.dun.lightRect[1] = gfxRectangle:new(0, 10, 224, 156)
		self.dun.lightRect[2] = gfxRectangle:new(0, 26, 224, 108)
		self.dun.lightRect[3] = gfxRectangle:new(0, 48, 224, 66)
	end


	btTable.addParent(self, __bigpic)
	btTable.setClassMetatable(self)

	readImages()
	makeRectangles()
	readCityImages()
	readDunImages()

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
-- cityBackground()
----------------------------------------
function bigpic:cityBackground()
	self.surface:Draw(nil, self.city.bg, nil)
end

function bigpic:cityAdd(inQuad, inFacet, inBuilding)
	local q

	if (self.city[inQuad][inFacet] == nil) then
		return
	end

	q = self.city[inQuad][inFacet]

	if (q[inBuilding] == nil) then
		self.surface:Draw(nil, self.imgs[inBuilding].img, nil)
	else
		self.surface:Draw(q.rect, q[inBuilding], nil)
	end
end

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

		if (globals.swapWallsAndDoors) then
			if (gfx == "door") then
				gfx = "wall"
			else
				gfx = "door"
			end
		end
	end

	q = self.dun[inQuad][inFacet]
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
























