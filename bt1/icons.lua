----------------------------------------
-- Icon definitions
----------------------------------------
local iconData = {
	{ 
		gfx		= "images/icon_light.bta",
		gfxType		= "bta",
		clear		= "images/icon_light_clear.png",
		clearType	= "png",
		x		= 256,
		y		= 22,
		w		= 80,
		h		= 38
	},
	{ 
		gfx		= "images/icon_levitate.bta",
		gfxType		= "bta",
		clear		= "images/icon_levitate_clear.png",
		clearType	= "png",
		x		= 256,
		y		= 66,
		w		= 80,
		h		= 44
	},
	{ 
		gfx		= "images/icon_detect.bta",
		gfxType		= "bta",
		clear		= "images/icon_detect_clear.png",
		clearType	= "png",
		x		= 256,
		y		= 166,
		w		= 80,
		h		= 28
	},
	{ 
		gfx		= "images/icon_shield.png",
		gfxType		= "png",
		clear		= "images/icon_shield_clear.png",
		clearType	= "png",
		x		= 256,
		y		= 202,
		w		= 80,
		h		= 34
	},
	{ 
		gfx		= {
			north = "images/icon_compass_north.png",
			south = "images/icon_compass_south.png",
			east = "images/icon_compass_east.png",
			west = "images/icon_compass_west.png",
		},
		gfxType		= "png",
		clear		= "images/icon_compass_clear.png",
		clearType	= "png",
		x		= 256,
		y		= 112,
		w		= 80,
		h		= 50
	}
}
		

icons = {}

----------------------------------------
-- Constants
----------------------------------------
icons.ICON_LIGHT	= 1
icons.ICON_LEVITATE	= 2
icons.ICON_DETECT	= 3
icons.ICON_SHIELD	= 4
icons.ICON_COMPASS	= 5

----------------------------------------
-- new()
----------------------------------------
function icons:new(inType)
	local self = {
		type		= inType,
		gfx		= false,
		clear		= false,
		rectangle	= false,
		active		= false
	}

	local data = iconData[inType]
	if (inType == icons.ICON_COMPASS) then
		self.gfx = {}
		self.gfx.north = gfxImage:new(data.gfx.north, data.gfxType)
		self.gfx.south = gfxImage:new(data.gfx.south, data.gfxType)
		self.gfx.east = gfxImage:new(data.gfx.east, data.gfxType)
		self.gfx.west = gfxImage:new(data.gfx.west, data.gfxType)
	else
		self.gfx = gfxImage:new(data.gfx, data.gfxType)
	end

	self.clear = gfxImage:new(data.clear, data.clearType)
	self.rectangle = gfxRect:New(data.x, data.y, data.w, data.h)

	btTable.addParent(self, icons)
	btTable.setClassMetatable(self)

	return self
end

function icons:activate()
	self.active = true
	self.gfx:Draw(m_window, self.rectangle)
end

function icons:deactivate()
	if (self.active) then
		self.active = false
		self.gfx:Clear()
	end

	self.clear:Draw(m_window, self.rectangle)
end

function icons:update(inDirection)
	if (self.type == icons.ICON_COMPASS) then
		self.gfx[inDirection]:Draw(m_window, self.rectangle)
		m_window:Update(self.rectangle)
	end
end

























