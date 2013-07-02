require "character"
require "monster"
require "battleSummon"

summon = {}

function summon:new(inName)
	local self = {
	}

	btTable.addParent(self, summon, battleSummon, monster:new(inName), 
				character, baseCharacter
			)
	btTable.setClassMetatable(self)

	self.cur_hp = self.curHP
	self.max_hp = self.maxHP

	self.__index = self

	return self
end

function summon:isSummon() 
	return true
end

function summon:isCharacter()
	return true
end

function summon:getStatusLine()
	local hpstring
	local outString
	local outHpString
	local myac
	local acstring

	myac = self.ac
	if (myac >= -9) then
		acstring = string.format("%2d", myac)
	else
		acstring = "L0"
	end

	outString = string.format("%-15s %2s %4d        0 ??",
		self.singular, acstring, self.max_hp
		)
	outHpString = string.format("%4d", self.cur_hp)

	return outString,outHpString
end

function summon:canBeAttacked()
	if ((self.isHostile) or (self.isDead) or (self.isStoned)) then
		return false
	end

	if (self.isIllusion) then
		local xxx_illusion_disbelieve_check
	end

	return true
end
