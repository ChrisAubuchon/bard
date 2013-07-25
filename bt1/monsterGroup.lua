require "dataio"
require "bttable"
require "entity"
require "random"
require "objectHash"
require "battleMonster"

monsterGroup = {}

function monsterGroup:new(inName, inSize)
	local self = {
		size		= 0,
		inMeleeRange	= false,
		name		= inName,
		isIllusion	= false
	}

	btTable.addParent(self, monsterGroup, 
				monster, 
				linkedList, 
				linkedListNode,
				monster.base(inName), 
				objectHash:new(self),
				battleMonster, 
				entity,
				battleBonus
				)
	btTable.setClassMetatable(self)

	local m

	if (not inSize) then
		if (not self.rndGroupSize) then
			error("rndGroupSize == false", 4)
		end

		inSize = rnd_xdy(1, self.groupSize)
	end

	for _ = 1,inSize do
		self:addMonster()
	end

	self.__index = self

	return self
end

function monsterGroup:addMonster()
	local m 	= monster:new(self.name)

	m.parentGroup = self
	btTable.addParent(m, battleMonster)

	self:insertTail(m)
	self.size = self.size + 1
end

function monsterGroup:removeMonster(inMonster)
	self:remove(inMonster)
	inMonster:deleteKey()
	self.size = self.size - 1
end

----------------------------------------
-- truncate()
--
-- Remove all members except for the
-- first
----------------------------------------
function monsterGroup:truncate()
	assert(self.head, "trucate()ng an empty monster group")

	self.head.next = false
	self.tail = self.head
end
