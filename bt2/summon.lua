summon = {}

----------------------------------------
-- summon:new()
----------------------------------------
function summon:new(inName, inIsIllusion)
	local self = {
		isIllusion	= inIsIllusion
	}

	dprint(inName)

	btTable.addParent(self, summon)
	btTable.addParent(self, battleParty)
	btTable.addParent(self,	monster:new(inName))
	btTable.addParent(self,	character)
	btTable.addParent(self,	bIICharacter)
	btTable.addParent(self,	battleBonus)
	btTable.addParent(self,	linkedListNode)
	btTable.setClassMetatable(self)

	self.baseAC	= self.ac
	self.cur_hp	= self.curHP
	self.max_hp	= self.maxHP
	self.name	= self.singular
	self.numAttacks	= 1

	if (self.isIllusion) then
		self.class = "Il"
	else
		self.class = "Mn"
	end

	self.__index = self

	return self
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

