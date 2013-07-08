local durationSpell = {}

durationSpell.new = function(inIcon)
	local self = {
		icon		= inIcon,
		active		= false,
		duration	= 0
	}

	function self.activate(inDuration)
		self.__activate(inDuration)
	end

	function self.__activate(inDuration)
		self.active = true
		if (inDuration > 0) then
			self.duration = inDuration + rnd_xdy(1,16)
		else
			self.duration = inDuration
		end
		icons.displayIcon(self.icon)
		printEllipsis()
	end

	function self.deactivate()
		self.__deactivate()
	end

	function self.__deactivate()
		self.active = false
		self.duration = false
		icons.deactivateIcon(self.icon)
	end

	function self.update(inData)
	end

	return self
end

lightEffect	= durationSpell.new("light")

lightEffect.activate = function(inDuration, inDistance, inSeeSecret)
	lightEffect.__activate(inDuration)
	lightEffect.distance	= inDistance
	lightEffect.seeSecret	= inSeeSecret

	if (party.lightSongPlaying) then
		party.singer:songTimeout()
	end

	if (currentLevel.isDungeon()) then
		currentLevel.buildView()
	end
end

lightEffect.deactivate = function ()
	lightEffect.distance = 0
	lightEffect.seeSecret = false
	lightEffect.__deactivate()
	if (currentLevel and currentLevel.isDungeon() 
			 and globals.doTimeEvents) then
		currentLevel.buildView()
	end
end

shieldEffect	= durationSpell.new("shield")
shieldEffect.bonus = 0

shieldEffect.activate = function(inDuration, inBonus)
	shieldEffect.bonus = inBonus
	shieldEffect.__activate(inDuration)
	party:display()
end

shieldEffect.deactivate = function()
	shieldEffect.bonus = 0
	shieldEffect.__deactivate()
	party:display()
end

detectEffect	= durationSpell.new("detect")
detectEffect.activate = function(inDuration, inStairs, inTraps, inSpecial)
	detectEffect.stairs	= inStairs
	detectEffect.traps	= inTraps
	detectEffect.special	= inSpecial
	detectEffect.__activate(inDuration)
end
detectEffect.deactivate = function()
	detectEffect.stairs	= false
	detectEffect.traps	= false
	detectEffect.special	= false
	detectEffect.__deactivate()
end

levitateEffect	= durationSpell.new("levitate")

compassEffect	= durationSpell.new("compass")
compassEffect.activate = function(inDuration)
	compassEffect.active = true
	if (inDuration > 0) then
		compassEffect.duration = inDuration + rnd_xdy(1,16)
	else
		compassEffect.duration = inDuration
	end
	printEllipsis()
end

compassEffect.update = function (inData)
	if (compassEffect.active) then
		icons.updateCompass(inData)
	end
end
