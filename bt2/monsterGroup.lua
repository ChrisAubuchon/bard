
monsterGroup = {}

function monsterGroup:new(inName, inSize)
	local self = object:new()

	self:addParent(monsterGroup)
	self:addParent(monster) 
	self:addParent(linkedList)
	self:addParent(linkedListNode)
	self:addParent(monsterData[inName]) 
	self:addParent(battleMonster)
	self:addParent(entity)
	self:addParent(battleBonus)

	self.size	= 0
	self.range	= self.distance
	self.name	= inName
	self.isIllusion	= false

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
	m:addParent(battleMonster)

	self:insertTail(m)
	self.size = self.size + 1
end

function monsterGroup:removeMonster(inMonster)
	self:remove(inMonster)
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
