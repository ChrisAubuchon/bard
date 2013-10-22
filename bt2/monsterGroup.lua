
monsterGroup = {}

function monsterGroup:new(inName, inSize)
	local self = {
		size		= 0,
		range		= false,
		name		= inName,
		isIllusion	= false
	}

	btTable.addParent(self, monsterGroup)
	btTable.addParent(self, monster) 
	btTable.addParent(self, linkedList)
	btTable.addParent(self, linkedListNode)
	btTable.addParent(self, monster.base(inName)) 
	btTable.addParent(self, objectHash:new(self))
	btTable.addParent(self, battleMonster)
	btTable.addParent(self, entity)
	btTable.addParent(self, battleBonus)
	btTable.setClassMetatable(self)
	self.range = self.distance

	local m

	if (not inSize) then
		if (not self.rndGroupSize) then
			error("rndGroupSize == false", 4)
		end

		inSize = random:xdy(1, self.groupSize)
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
