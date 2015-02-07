require "bIIICharacter"

character = {}

----------------------------------------
-- character:new()
----------------------------------------
function character:new()
	local self	= object:new()

	self:addParent(bIIICharacter)
	self:addParent(character)
	self:addParent(classes)
	self:addParent(entity)

	return self
end

----------------------------------------
-- character:fromTable()
----------------------------------------
function character:fromTable(inTable)
	local rval	= character:new()
	local key
	local value

	for key,value in pairs(inTable) do
		if (key ~= "inventory") then
			if (type(value) == "table") then
				rval[key] = table.copy(value)
			else
				rval[key] = value
			end
		else
			rval.inventory = inventory:new()
			rval.inventory:fromTable(value)
		end
	end

	if (not rval.inventory) then
		rval.inventory = inventory:new()
	end

	return rval
end

----------------------------------------
-- character:getStatusLine()
----------------------------------------
function character:getStatusLine()
	local rval	= {}

	self:calcAC()

	rval.name	= self.name
	rval.ac		= sprintf("%3d", self.ac)

	if (self.isDead) then
		rval.maxhp	= "Dead"
	elseif (self.isOld) then
		rval.maxhp	= "Old "
	elseif (self.isPoisoned) then
		rval.maxhp	= "Psnd"
	elseif (self.isStoned) then
		rval.maxhp	= "Ston"
	elseif (self.isParalyzed) then
		rval.maxhp	= "Para"
	elseif (self.isPossessed) then
		rval.maxhp	= "Poss"
	elseif (self.isNuts) then
		rval.maxhp	= "Nuts"
	else
		rval.maxhp	= sprintf("%3d", self.maxHp)
	end

	rval.curhp	= sprintf("%3d", self.currentHp)
	rval.maxsppt	= sprintf("%3d", self.maxSppt)
	rval.cursppt	= sprintf("%3d", self.currentSppt)
	rval.class	= string.sub(self.class, 1, 2)

	return rval
end

local dexterityACBonus = {
	1, 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5
}

----------------------------------------
-- character:calcAC()
----------------------------------------
function character:calcAC()
	self.ac	= 10

	-- XXX - Calculate song bonus

	for i in self.inventory:iterator() do
		if (i.isEquipped) then
			self.ac = self.ac - i.ac_bonus
		end
	end

	if (self.dx >= 14) then
		-- Dexterity can't be higher than 30
		if (self.dx > 30) then
			self.dx = 30
		end

		-- "+ 1" since lua array indexes start at 1
		self.ac = self.ac - dexterityACBonus[self.dx - 14 + 1]
	end

	-- A monk gets -1 to AC for every level up to 255
	--
	if (self.class == "Monk") then
		if (self.cur_level > 255) then
			self.ac = self.ac - 255
		else
			self.ac = self.ac - self.cur_level
		end
	end

	-- XXX - subtract passive effect shield bonus

	-- XXX - Add combat ac bonuses/penalties

	if (self.ac < -50) then
		self.ac = -50
	end
end

----------------------------------------
-- character:print()
----------------------------------------
function character:print()
	bigpic:drawImage(self.pic)
	bigpic:setTitle(self.name)

	local function printStats()
		text:clear()

		text:print("%s is a Level %3d %s %s %s", 
			self.name, self.cur_level, self.gender, self.race,
			self.class
			)

	end

	printStats()

	getkey()
end
