declare_global("icons")

local gfx = {};
local clear = {};
local rects = {};
local active = {};

local function __init()
	gfx.light = load_image("images/icon_light.bta", "bta");
	gfx.levitate = load_image("images/icon_levitate.bta", "bta");
	gfx.detect = load_image("images/icon_detect.bta", "bta");
	gfx.shield = load_image("images/icon_shield.png", "png");
	
	gfx.compass = {};
	gfx.compass["north"] = load_image("images/icon_compass_north.png", "png");
	gfx.compass["south"] = load_image("images/icon_compass_south.png", "png");
	gfx.compass["east"] = load_image("images/icon_compass_east.png", "png");
	gfx.compass["west"] = load_image("images/icon_compass_west.png", "png");
	
	
	clear.light = load_image("images/icon_light_clear.png", "png");
	clear.levitate = load_image("images/icon_levitate_clear.png", "png");
	clear.shield = load_image("images/icon_shield_clear.png", "png");
	clear.detect = load_image("images/icon_detect_clear.png", "png");
	clear.compass = load_image("images/icon_compass_clear.png", "png");
	
	
	rects.light = video_rectangle(256, 22, 80, 38);
	rects.levitate = video_rectangle(256, 66, 80, 44);
	rects.compass = video_rectangle(256, 112, 80, 50);
	rects.detect = video_rectangle(256, 166, 80, 28);
	rects.shield = video_rectangle(256, 202, 80, 34);
	
	active.light = 0;
	active.levitate = 0;
	active.compass = 0;
	active.detect = 0;
	active.shield = 0;
end

local function displayIcon(i)
	if (gfx[i] == nil) then
		return;
	end

	if (gfx[i].isAnim == 1) then
		active[i] = 1;
		gfx[i]:start(m_window, rects[i]);
	else
		m_window:Blit(rects[i], gfx[i], nil);
		m_window:UpdateRect(rects[i]);
	end
end

local function deactivateIcon(i)
	if ((gfx[i].isAnim == 1) and (active[i] == 1)) then
		active[i] = 0;
		gfx[i]:stop();
	end

	m_window:Blit(rects[i], clear[i], nil);
	m_window:UpdateRect(rects[i]);
end

local function updateCompass(dir)
	m_window:Blit(rects.compass, gfx.compass[dir], nil);
	m_window:UpdateRect(rects.compass);

end

__init()


icons = {
	displayIcon = displayIcon,
	deactivateIcon = deactivateIcon,
	updateCompass = updateCompass
};
