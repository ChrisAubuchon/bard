require "dataio"
require "bttable"
require "entity"
require "random"
require "objectHash"
require "battleMonster"

monster = {}
monsterGroup = {}
monsterParty = {}

local _monsters
_monsters = read_table("monsters")

function monster:new(inName)
	local self = {
		beenAttacked	= false,
		maxHP		= 0,
		curHP		= 0,
		ac		= 10,
		parentGroup	= false,
	}

	btTable.addParent(self, monster, _monsters[inName], entity,
			objectHash:new(self)
			)
	btTable.setClassMetatable(self)

	self.maxHP = self.hpBase + rnd_xdy(self.hpRndNDice, self.hpRndDie)
	self.curHP = self.maxHP
	self.ac		= self.baseAC

	self.__index = self

	return self
end

function monster:isMonster()
	return true
end

function monster:getSingularName()
	return self.singular
end

function monster:getBattlePriority()
	return rnd_between_xy_inc(self.priorityLo, self.priorityHi)
end

function monster:getPronoun()
	return self.pronoun
end

function monster:inMeleeRange()
	return self.parentGroup.inMeleeRange
end

--------------------------------------------------------------------------------

function monsterGroup:new(inName, inSize)
	local self = {
		inMeleeRange	= false,
		inName		= inName,
		name		= inName,
	}

	btTable.addParent(self, monsterGroup, monster, btArray, 
				entity, _monsters[inName], objectHash:new(self),
				battleMonster
				)
	btTable.setClassMetatable(self)

	local size
	local m

	if (inSize) then
		size = inSize
	else
		if (not self.rndGroupSize) then
			error("Creating random monster group size with rndGroupsize = false", 4)
		end

		size = rnd_xdy(1, self.groupSize)
	end

	for _ = 1,size do
		self:addMonster()
	end

	self.__index = self

	return self
end

function monsterGroup:addMonster()
	local m = monster:new(self.inName)
	m.parentGroup = self
	btTable.addParent(m, battleMonster)
	self:__add(m)
end

function monsterGroup:removeMonster(inMonster)
	local m
	local mon

	for m,mon in self:ipairs() do
		if (mon == inMonster) then
			self:deleteKey()
			self:__remove(m)
			return
		end
	end

	error("monsterGroup:removeMonster(): Monster not found")
end

--------------------------------------------------------------------------------

function monsterParty:new()
	local self = {
	}
	btTable.addParent(self, monsterParty, btArray)
	btTable.setClassMetatable(self)

	self.__index = self

	return self
end

function monsterParty:addMonsterGroup(inGroup)
	self:__add(inGroup)
	self:adjustMeleeGroups()
end

function monsterParty:removeMonsterGroup(inGroup)
	local mgroup
	local i
	local groupIndex

	for i,mgroup in self:ipairs() do
		if (mgroup == inGroup) then
			mgroup:deleteKey()
			groupIndex = i
			break
		end
	end

	dprint("Group index to delete: %d", groupIndex)
	self:__remove(groupIndex)
end

function monsterParty:adjustMeleeGroups()
	local i

	for i = 1,2 do
		if (self[i]) then self[i].inMeleeRange = true end
	end

	for i = 3,self.size do
		self[i].inMeleeRange = false
	end
end

function monsterParty:isGroup(inIndex)
	return self[inIndex]
end

function monsterParty:getLeadGroup()
	return self[1]
end

--------------------------------------------------------------------------------

function monster.getXPReward(inType)
	assert(_monsters[inType])

	return _monsters[inType].reward
end

function monster.pluralizeName(inType, numMonsters)
	if (numMonsters == 1) then
		return _monsters[inType].singular
	else
		return _monsters[inType].plural
	end
end
