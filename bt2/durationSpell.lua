----------------------------------------
-- durationSpell()
--
-- Base class for constant effect spells
----------------------------------------
local durationSpell = {}
function durationSpell:new(inIcon)
	local self = object:new()

	self:addParent(durationSpell)

	self.active	= false
	self.icon	= icons:new(inIcon)
	self.duration	= 0

	return self
end

----------------------------------------
-- durationSpell:__activate()
----------------------------------------
function durationSpell:__activate(inDuration)
	if (inDuration > 0) then
		self.duration = inDuration + random:xdy(1,16)
	else
		self.duration = inDuration
	end
	self.icon:activate()
end

----------------------------------------
-- durationSpell:__deactivate()
----------------------------------------
function durationSpell:__deactivate()
	self.duration = 0
	self.icon:deactivate()
end

----------------------------------------
-- lightEffect class
----------------------------------------
lightEffect = {}
function lightEffect:new()
	local self = durationSpell:new(icons.ICON_LIGHT)

	self:addParent(lightEffect)

	self.distance	= 0
	self.seeSecret	= false

	return self
end

----------------------------------------
-- lightEffect:activate()
----------------------------------------
function lightEffect:activate(inDuration, inDistance, inSeeSecret)
	self.active	= true
	self.distance	= inDistance
	self.seeSecret	= inSeeSecret

	self:__activate(inDuration)

	if (party.song.lightSong) then
		party.song.singer:songTimeout()
	end

	if (currentLevel:isDungeon()) then
		currentLevel:buildView()
	end
	text:printEllipsis()
end

----------------------------------------
-- lightEffect:deactivate()
----------------------------------------
function lightEffect:deactivate()
	self.active	= false
	self.distance	= 0
	self.seeSecret	= false
	self:__deactivate()
	if (currentLevel and currentLevel:isDungeon() 
			 and globals.doTimeEvents) then
		currentLevel:buildView()
	end
end

----------------------------------------
-- shieldEffect class
----------------------------------------
shieldEffect = {}
function shieldEffect:new()
	self = durationSpell:new(icons.ICON_SHIELD)

	self:addParent(shieldEffect)

	self.bonus = 0

	return self
end

----------------------------------------
-- shieldEffect:activate()
----------------------------------------
function shieldEffect:activate(inDuration, inBonus)
	self.active	= true
	self.bonus	= inBonus
	self:__activate(inDuration)
	party:display()
	text:printEllipsis()
end

----------------------------------------
-- shieldEffect:deactivate()
----------------------------------------
function shieldEffect:deactivate()
	self.active	= false
	self.bonus	= 0
	self:__deactivate()
	party:display()
end

----------------------------------------
-- detectEffect class
----------------------------------------
detectEffect = {}
function detectEffect:new()
	local self = durationSpell:new(icons.ICON_DETECT)

	self:addParent(detectEffect)

	self.stairs	= false
	self.traps	= false
	self.special	= false

	return self
end

----------------------------------------
-- detectEffect:activate()
----------------------------------------
function detectEffect:activate(inDuration, inStairs, inTraps, inSpecial)
	self.active	= true
	self.stairs	= inStairs
	self.traps	= inTraps
	self.special	= inSpecial
	self:__activate(inDuration)
	text:printEllipsis()
end

----------------------------------------
-- detectEffect:deactivate()
----------------------------------------
function detectEffect:deactivate()
	self.active	= false
	self.stairs	= false
	self.traps	= false
	self.special	= false
	self:__deactivate()
end

----------------------------------------
-- levitateEffect class
----------------------------------------
levitateEffect = {}
function levitateEffect:new()
	local self = durationSpell:new(icons.ICON_LEVITATE)

	self:addParent(levitateEffect)

	return self
end

----------------------------------------
-- levitateEffect:activate()
----------------------------------------
function levitateEffect:activate(inDuration)
	self.active	= true
	self:__activate(inDuration)
	text:printEllipsis()
end

----------------------------------------
-- levitateEffect:deactivate()
----------------------------------------
function levitateEffect:deactivate()
	self.active	= false
	self:__deactivate()
end

----------------------------------------
-- compassEffect class
----------------------------------------
compassEffect = {}
function compassEffect:new()
	local self = durationSpell:new(icons.ICON_COMPASS)

	self:addParent(compassEffect)

	return self
end

----------------------------------------
-- compassEffect:activate()
----------------------------------------
function compassEffect:activate(inDuration)
	self.active	= true
	if (inDuration > 0) then
		self.duration = inDuration + random:xdy(1,16)
	else
		self.duration = inDuration
	end
	text:printEllipsis()
end

----------------------------------------
-- compassEffect:deactivate()
----------------------------------------
function compassEffect:deactivate()
	self.active	= false
	self:__deactivate()
end

----------------------------------------
-- compassEffect:update()
----------------------------------------
function compassEffect:update(inDirection)
	if (self.active) then
		self.icon:update(inDirection)
	end
end
		






















