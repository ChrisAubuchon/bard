require "dataio"
require "bttable"
require "entity"
require "random"
require "objectHash"
require "battleMonster"

monster = {}

local _monsters
_monsters = read_table("monsters")

local m
local a

for _,m in pairs(_monsters) do
	for _,a in ipairs(m.attacks) do
		compileAction(a.action)
	end
end

function monster:new(inName)
	local self = {
		beenAttacked	= false,
		maxHP		= 0,
		curHP		= 0,
		ac		= 10,
		parentGroup	= false,
	}

	btTable.addParent(self, monster, 
			_monsters[inName], 
			entity,
			objectHash:new(self),
			linkedListNode
			)
	btTable.setClassMetatable(self)

	self.maxHP = self.hpBase + rnd_xdy(self.hpRndNDice, self.hpRndDie)
	self.curHP = self.maxHP
	self.ac		= self.baseAC

	self.__index = self

	return self
end

function monster:getMultiString()
	return "One"
end

function monster:compare(inValue)
	if (self.key == inValue) then
		return true
	end

	return false
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

function monster:attackIterator()
	local count 	= 0
	local index	= 0

	local function f(_)
		count = count + 1
		if (count > 4) then
			return
		end
		if (index == 0) then
			index = rnd_xdy(1,4)
		else
			index = index + 1
			if (index > 4) then	
				index = 1
			end
		end
		return self.attacks[index]
	end
	return f, nil, nil
end

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


function monster.typeTable()
	local t = {}

	function t:toArray() 
		local r = {}

		for i,_ in pairs(_monsters) do
			table.insert(r, i)
		end

		table.sort(r)
		return r
	end

	return t
end

function monster.base(inType)
	return _monsters[inType]
end
