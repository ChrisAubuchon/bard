declare_global("icons")
local iconGfx = {};
local clear = {};
local rects = {};
local active = {};

local function __init()
	iconGfx.light = gfxImage:new("images/icon_light.bta", "bta");
	iconGfx.levitate = gfxImage:new("images/icon_levitate.bta", "bta");
	iconGfx.detect = gfxImage:new("images/icon_detect.bta", "bta");
	iconGfx.shield = gfxImage:new("images/icon_shield.png", "png");
	
	iconGfx.compass = {};
	iconGfx.compass["north"] = gfxImage:new("images/icon_compass_north.png", "png");
	iconGfx.compass["south"] = gfxImage:new("images/icon_compass_south.png", "png");
	iconGfx.compass["east"] = gfxImage:new("images/icon_compass_east.png", "png");
	iconGfx.compass["west"] = gfxImage:new("images/icon_compass_west.png", "png");
	
	
	clear.light = gfxImage:new("images/icon_light_clear.png", "png");
	clear.levitate = gfxImage:new("images/icon_levitate_clear.png", "png");
	clear.shield = gfxImage:new("images/icon_shield_clear.png", "png");
	clear.detect = gfxImage:new("images/icon_detect_clear.png", "png");
	clear.compass = gfxImage:new("images/icon_compass_clear.png", "png");
	
	
	rects.light = gfxRectangle:new(256, 22, 80, 38);
	rects.levitate = gfxRectangle:new(256, 66, 80, 44);
	rects.compass = gfxRectangle:new(256, 112, 80, 50);
	rects.detect = gfxRectangle:new(256, 166, 80, 28);
	rects.shield = gfxRectangle:new(256, 202, 80, 34);
	
	active.light = false;
	active.levitate = false;
	active.compass = false;
	active.detect = false;
	active.shield = false;
end

local function displayIcon(i)
	if (iconGfx[i] == nil) then
		return;
	end

	active[i] = true
	iconGfx[i]:Draw(m_window, rects[i])
end

local function deactivateIcon(i)
	if (active[i]) then
		active[i] = false
		iconGfx[i]:Clear()
	end

	m_window:Draw(rects[i], clear[i], nil)
end

local function updateCompass(dir)
	m_window:Draw(rects.compass, iconGfx.compass[dir], nil);
	m_window:Update(rects.compass);

end

__init()


icons = {
	displayIcon = displayIcon,
	deactivateIcon = deactivateIcon,
	updateCompass = updateCompass
};
