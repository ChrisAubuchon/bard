summon = {}

----------------------------------------
-- summon:new()
----------------------------------------
function summon:new(inName, inIsIllusion)
	local self = {
		isIllusion	= inIsIllusion,
		monsterType	= inName
	}


	btTable.addParent(self, summon)
	btTable.addParent(self, battleParty)
	btTable.addParent(self,	monster:new(inName))
	btTable.addParent(self,	character)
	btTable.addParent(self,	bIICharacter)
	btTable.addParent(self,	battleBonus)
	btTable.addParent(self,	linkedListNode)
	btTable.addParent(self, entity)
	btTable.setClassMetatable(self)

	self.baseAC	= self.ac
	self.name	= self.singular
	self.inventory	= inventory:new()
	self.currentHp	= self.curHP
	self.maxHp	= self.maxHP

	if (self.isIllusion) then
		self.class = "Il"
	else
		self.class = "Mn"
	end

	self.__index = self

	return self
end

----------------------------------------
-- summon:toTable()
----------------------------------------
function summon:toTable()
	local t		= {}

	local function toTable(inSelf, inDest, inHeader)
		local k
		local v

		for k,_ in pairs(inHeader) do
			v = inSelf[k]

			if (type(v) == "function") then
				local doNothing = 1
			elseif (type(v) == "table") then
				if (v.toTable) then
					inDest[k] = v:toTable()
				else
					inDest[k] = table.copy(v)
				end
			else
				inDest[k] = inSelf[k]
			end
		end
	end

	toTable(self, t, entity)
	t.isIllusion = self.isIllusion
	t.monsterType = self.monsterType

	return t
end

----------------------------------------
-- summon:fromTable()
----------------------------------------
function summon:fromTable(inTable)
	local rval
	local k
	local v

	rval = summon:new(inTable.monsterType)

	for k,v in pairs(inTable) do
		if (k ~= "inventory") then
			if (type(v) == "table") then
				rval[k] = table.copy(v)
			else
				rval[k] = v
			end
		else
			rval.inventory = inventory:new()
			rval.inventory:fromTable(v)
		end
	end

	if (not rval.inventory) then
		rval.inventory = inventory:new()
	end

	return rval
end

----------------------------------------
-- summon:isSummon()
----------------------------------------
function summon:isSummon()
	return true
end

----------------------------------------
-- summon:isCharacter()
----------------------------------------
function summon:isCharacter()
	return true
end

----------------------------------------
-- summon:getMultiString()
----------------------------------------
function summon:getMultiString()
	return self.singular
end

----------------------------------------
-- summon:calcAC()
----------------------------------------
function summon:calcAC()
	self.ac = self.baseAC

	if (party.shield.active) then
		self.ac = self.ac - party.shield.bonus
	end

	if (party.song.acBonus) then
		self.ac = self.ac - party.song.acBonus
	end

	self.ac = self.ac - party.acBonus
	self.ac = self.ac - self.acBonus

	if (self.ac > 10) then self.ac = 10 end
	if (self.ac < -21) then self.ac = -21 end
end

----------------------------------------
-- findMeleeAttack()
----------------------------------------
function summon:findMeleeAttack()
	local i

	for _,i in ipairs(self.attacks) do
		if (i.type == "melee") then
			return i.action.inData
		end
	end

	return false
end

local specialString = {
	poison		= "Poison",
	drain		= "Level drain",
	nuts		= "Insanity",
	old		= "Aging",
	possess		= "Possession",
	stone		= "Stoned",
	critical	= "Critical hit",
	spptdrain	= "Point Phaze",
	unequip		= "Item-zot"
}

----------------------------------------
-- summon:setBigpic()
----------------------------------------
function summon:setBigpic()
	bigpic:setBigpic(self.picture, self.name)
end

----------------------------------------
-- summon:printAttributeScreen()
----------------------------------------
function summon:printAttributeScreen()
	local melee

	melee = self:findMeleeAttack()
	if (not melee) then
		melee = {}
		melee.ndice = 1
		melee.specialAttack = false
	end

	text:clear()
	text:print(self.name)
	text:print("\nClass: ")
	if (self.class == "Il") then
		text:print("Illusion")
	else
		text:print("Monster")
	end
	text:print("\nDamage Range: %d-%d", 
		melee.ndice,
		melee.ndice * 4)
	text:print("\nNumber of attacks: %d",
		self.numAttacks)
	text:print("\nSpecial: ")
	if (melee.specialAttack) then
		text:print(specialString[melee.specialAttack])
	else
		text:print("Normal")
	end
end

