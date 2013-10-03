local durationSpell = {}
function durationSpell:new(inIcon)
	local self = {
		icon		= icons:new(inIcon),
		duration	= 0
	}

	btTable.addParent(self, durationSpell)
	btTable.setClassMetatable(self)

	return self
end

function durationSpell:__activate(inDuration)
	if (inDuration > 0) then
		self.duration = inDuration + rnd_xdy(1,16)
	else
		self.duration = inDuration
	end
	self.icon:activate()
end

function durationSpell:__deactivate()
	self.duration = 0
	self.icon:deactivate()
end

lightEffect = {}
function lightEffect:new()
	local self = {
		active		= false,
		distance	= 0,
		seeSecret	= false,
		effect		= durationSpell:new(icons.ICON_LIGHT)
	}

	btTable.addParent(self, lightEffect, self.effect)
	btTable.setClassMetatable(self)

	return self
end

function lightEffect:activate(inDuration, inDistance, inSeeSecret)
	self.active	= true
	self.distance	= inDistance
	self.seeSecret	= inSeeSecret

	self.effect:__activate(inDuration)

	if (party.song.lightSong) then
		party.song.singer:songTimeout()
	end

	if (currentLevel:isDungeon()) then
		currentLevel:buildView()
	end
	text:printEllipsis()
end

function lightEffect:deactivate()
	self.active	= false
	self.distance	= 0
	self.seeSecret	= false
	self.effect:__deactivate()
	if (currentLevel and currentLevel:isDungeon() 
			 and globals.doTimeEvents) then
		currentLevel:buildView()
	end
end

shieldEffect = {}
function shieldEffect:new()
	local self = {
		active		= false,
		bonus		= 0,
		effect		= durationSpell:new(icons.ICON_SHIELD)
	}

	btTable.addParent(self, shieldEffect, self.effect)
	btTable.setClassMetatable(self)

	return self
end

function shieldEffect:activate(inDuration, inBonus)
	self.active	= true
	self.bonus	= inBonus
	self.effect:__activate(inDuration)
	party:display()
	text:printEllipsis()
end

function shieldEffect:deactivate()
	self.active	= false
	self.bonus	= 0
	self.effect:__deactivate()
	party:display()
end

detectEffect = {}
function detectEffect:new()
	local self = {
		active		= false,
		stairs		= false,
		traps		= false,
		special		= false,
		effect		= durationSpell:new(icons.ICON_DETECT)
	}

	btTable.addParent(self, detectEffect, self.effect)
	btTable.setClassMetatable(self)

	return self
end

function detectEffect:activate(inDuration, inStairs, inTraps, inSpecial)
	self.active	= true
	self.stairs	= inStairs
	self.traps	= inTraps
	self.special	= inSpecial
	self.effect:__activate(inDuration)
	text:printEllipsis()
end

function detectEffect:deactivate()
	self.active	= false
	self.stairs	= false
	self.traps	= false
	self.special	= false
	self.effect:__deactivate()
end

levitateEffect = {}
function levitateEffect:new()
	local self = {
		active		= false,
		effect		= durationSpell:new(icons.ICON_LEVITATE)
	}

	btTable.addParent(self, levitateEffect, self.effect)
	btTable.setClassMetatable(self)

	return self
end

function levitateEffect:activate(inDuration)
	self.active	= true
	self.effect:__activate(inDuration)
	text:printEllipsis()
end

function levitateEffect:deactivate()
	self.active	= false
	self.effect:__deactivate()
end

compassEffect = {}
function compassEffect:new()
	local self = {
		active		= false,
		effect		= icons:new(icons.ICON_COMPASS)
	}

	btTable.addParent(self,compassEffect)
	btTable.setClassMetatable(self)

	return self
end

function compassEffect:activate(inDuration)
	self.active	= true
	if (inDuration > 0) then
		self.duration = inDuration + rnd_xdy(1,16)
	else
		self.duration = inDuration
	end
	text:printEllipsis()
end

function compassEffect:deactivate()
	self.active	= false
	self.effect:deactivate()
end

function compassEffect:update(inDirection)
	if (self.active) then
		self.effect:update(inDirection)
	end
end
		






















