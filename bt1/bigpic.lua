require "dataio"
require "sdl_video"
require "globals"

local rect
local active_bigpic
local view
local bp			-- bigpic images and animations
local city_quad			-- City quadrant table
local cv_rect			-- City view rectangles
local dun_quad			-- dungeon quadrant table
local dun_rect			-- dungeon view rectangles

----------------------------------------
-- setTitle
--
-- Set the title of the active bigpic
----------------------------------------
local function setTitle(format, ...)
	local title;
	local f;
	local surface;
	local w;
	local h;
	local x;

	if (format == nil) then
		error("setTitle() with a nil format", 2)
	end

	title = string.format(format, ...)

	f = globals.fonts.var;

	--w, h = f:SizeText(title);
	w,h = f:Size(title)
	if (w > 224) then
		return;
	end

	--surface = f:RenderText(title, globals.colors[16]);
	surface = f:Render(title, globals.colors[16])
	x = ((224 - w) / 2) + 36;

	m_window:Fill(gfxRectangle:new(32,212,224,16), globals.colors[1])
	m_window:Draw(gfxRectangle:new(x,212,0,0), surface, nil)
	m_window:Update(gfxRectangle:new(32,212,224,16))
end

----------------------------------------
-- drawBigpic()
--
-- Draw the bigpic
----------------------------------------
local function drawBigpic(i)
	if (active_bigpic ~= nil) then
		active_bigpic:Clear()
	end
	active_bigpic = nil

	if (bp[i] == nil) then
		return;
	end

	active_bigpic = bp[i].img
	active_bigpic:Draw(m_window, rect)
end

----------------------------------------
-- drawBigpicTimeAware()
--
-- Draw a bigpic image being aware of
-- the current time of day. The city
-- gates, for example darkens the sky
-- based on the time of day
----------------------------------------
local function drawBigpicTimeAware(i)
	if (active_bigpic ~= nil) then
		active_bigpic:Clear()
		active_bigpic = nil
	end

	view:Draw(nil, bp[i].img, nil)
	view:Update()
	if (globals.isNight) then
		view:SetColor(11, globals.colors[1])
	end
	m_window:Draw(rect, view, nil)
	m_window:Update()

	if (globals.isNight) then
		view:SetColor(11, globals.colors[12])
	end
end

----------------------------------------
-- city_add()
--
-- Add a section of the city graphics
-- to the view
----------------------------------------
local function city_add(quad, facet, building)
	local q

	assert(city_quad[quad] ~= nil, "Quadrant: " .. quad)

	if (city_quad[quad][facet] == nil) then
		return
	end

	q = city_quad[quad][facet]

	if (q[building] == nil) then
		view:Draw(nil, bp[building].img, nil)
	else
		view:Draw(q.rect, q[building], nil)
	end
end

----------------------------------------
-- dun_add()
--
-- Add a section of the dungeon graphics
-- to the view
----------------------------------------
local function dun_add(quad, tileSet, facet, sq)
	local gfx
	local q

	assert(dun_quad[quad] ~= nil, "Quadrant: " .. tostring(quad))

	if (dun_quad[quad][facet] == nil) then
		return
	end

	if (facet == "portal") then
		if (sq.hasCeilPortal) then
			gfx = "ceil"
		end
		if (sq.hasFloorPortal) then
			gfx = "floor"
		end
	else
		-- Add in flag check for swapping walls and doors here
		if ((sq.secret) and (party.light.seeSecret)) then
			gfx = "door"
		else
			gfx = sq.gfx
		end
	end

	q = dun_quad[quad][facet]
	view:Draw(q.rect, q[gfx][tileSet], nil)
end

----------------------------------------
-- city_background()
--
-- Set the background for the city view
----------------------------------------
local function city_background()
	view:Draw(nil, city_quad.bg, nil)
end

----------------------------------------
-- dun_background()
--
-- Set the background for the dungeon
-- view
----------------------------------------
local function dun_background(tileSet)
	if (not party.light.active) then
		view:Fill(gfxRectangle:new(0, 0, 224, 176), globals.colors[1])
		return
	end

	view:Draw(nil, dun_quad.bg[tileSet], nil)
	if (party.light.distance == 1) then
		view:Fill(gfxRectangle:new(0, 10, 224, 156), globals.colors[1])
	elseif (party.light.distance == 2) then
		view:Fill(gfxRectangle:new(0, 26, 224, 108), globals.colors[1])
	elseif (party.light.distance == 3) then
		view:Fill(gfxRectangle:new(0, 48, 224, 66), globals.colors[1])
	end
end

----------------------------------------
-- city_display()
--
-- Display the city view
----------------------------------------
local function city_display()
	if (active_bigpic ~= nil) then
		active_bigpic:Clear()
		active_bigpic = nil;
	end

	view:Update()
	if (globals.isNight) then
		view:SetColor(11, globals.colors[1])
	end
	m_window:Draw(rect, view, nil)
	m_window:Update()

	if (globals.isNight) then
		view:SetColor(11, globals.colors[12])
	end
end

----------------------------------------
-- dun_display()
--
-- Display the dungeon view
----------------------------------------
local function dun_display()
	if (active_bigpic ~= nil) then
		active_bigpic:Clear()
		active_bigpic = nil
	end

	view:Update()
	m_window:Draw(rect, view, nil)
	m_window:Update()
end

local function __init()
	local bpl
	local b
	local k1
	local k2
	local v1
	local v2

	bp = read_table("bigpic")
	for bpl,b in pairs(bp) do
		b.img = gfxImage:new(b.path, b.type)
	end

	rect = gfxRectangle:new(32, 30, 224, 176);

	city_quad = read_table("cityview")

	for k1,v1 in pairs(city_quad) do
		for k2, v2 in pairs(v1) do
			v2.rect = gfxRectangle:new(v2.x, v2.y, v2.w, v2.h);
			v2.building1 = gfxImage:new(
				"images/citypics/"..k1.."/"..k2.."/building1.png", "png")
			v2.building2 = gfxImage:new(
				"images/citypics/"..k1.."/"..k2.."/building2.png", "png")
			v2.building3 = gfxImage:new(
				"images/citypics/"..k1.."/"..k2.."/building3.png", "png")
			v2.building4 = gfxImage:new(
				"images/citypics/"..k1.."/"..k2.."/building4.png", "png")
		end
	end

	city_quad["bg"] = gfxImage:new("images/citypics/citybg.png", "png")

	dun_quad = read_table("dunview")
	for k1,v1 in pairs(dun_quad) do
		for k2, v2 in pairs(v1) do
			local path

			path = "images/dpics/"..k1.."/"..k2

			v2.rect = gfxRectangle:new(v2.x, v2.y, v2.w, v2.h)
			if (k2 == "portal") then
				v2.floor = {}
				v2.floor[0] = gfxImage:new(path.."/0-floor.png", "png")
				v2.floor[1] = gfxImage:new(path.."/1-floor.png", "png")
				v2.floor[2] = gfxImage:new(path.."/2-floor.png", "png")
				v2.ceiling = {}
				v2.ceiling[0] = gfxImage:new(path.."/0-ceiling.png", "png")
				v2.ceiling[1] = gfxImage:new(path.."/1-ceiling.png", "png")
				v2.ceiling[2] = gfxImage:new(path.."/2-ceiling.png", "png")
			else
				v2.door = {}
				v2.door[0] = gfxImage:new(path.."/0-door.png", "png")
				v2.door[1] = gfxImage:new(path.."/1-door.png", "png")
				v2.door[2] = gfxImage:new(path.."/2-door.png", "png")
				v2.wall = {}
				v2.wall[0] = gfxImage:new(path.."/0-wall.png", "png")
				v2.wall[1] = gfxImage:new(path.."/1-wall.png", "png")
				v2.wall[2] = gfxImage:new(path.."/2-wall.png", "png")
			end
		end
	end
	dun_quad["bg"] = {}
	dun_quad.bg[0] = gfxImage:new("images/dpics/0-bg.png", "png")
	dun_quad.bg[1] = gfxImage:new("images/dpics/1-bg.png", "png")
	dun_quad.bg[2] = gfxImage:new("images/dpics/2-bg.png", "png")

	view = gfxWindow:new(224, 176, 8)
end

__init()

bigpic = {
	city_add		= city_add,
	city_background		= city_background,
	city_display		= city_display,
	dun_add			= dun_add,
	dun_background		= dun_background,
	dun_display		= dun_display,
	drawBigpic		= drawBigpic,
	drawBigpicTimeAware	= drawBigpicTimeAware,
	setTitle		= setTitle
};
