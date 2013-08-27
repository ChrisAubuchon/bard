require "character"
require "monster"
require "battleSummon"
require "linkedList"

summon = {}

function summon:new(inName)
	local self = {
		baseAC		= 10,
		isHostile	= false,
		isIllusion	= false
	}

	btTable.addParent(self, summon, 
				battleParty,
				battleSummon, 
				monster:new(inName), 
				character, 
				baseCharacter, 
				battleBonus,
				linkedListNode
			)
	btTable.setClassMetatable(self)

	self.baseAC	= self.ac
	self.cur_hp	= self.curHP
	self.max_hp	= self.maxHP
	self.name	= self.singular
	self.numAttacks	= 1

	self.__index = self

	return self
end

function summon:isSummon() 
	return true
end

function summon:isCharacter()
	return true
end

function summon:getMultiString()
	return self.singular
end

----------------------------------------
-- calcAC()
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

	if (self.ac < -10) then self.ac = -10 end
	if (self.ac >  10) then self.ac =  10 end
end

----------------------------------------
-- getStatusLine()
----------------------------------------
function summon:getStatusLine()
	local outString
	local outHpString

	outString = string.format("%-15s %2s %4s        0 ??",
		self.singular,
		self:getAcString(),
		self:getHpString()
		)

	outHpString = string.format("%4d", self.cur_hp)

	return outString,outHpString
end

----------------------------------------
-- canBeAttacked()
----------------------------------------
function summon:canBeAttacked()
	if ((self.isHostile) or (self.isDead) or (self.isStoned)) then
		return false
	end

	if (self.isIllusion) then
		local xxx_illusion_disbelieve_check
	end

	return true
end
