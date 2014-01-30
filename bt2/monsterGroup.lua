
monsterGroup = {}

function monsterGroup:new(inName, inSize)
	local self = linkedList:new()

	self.action	= btAction:new()

	self:addParent(monsterGroup)
	self:addParent(monster) 
	self:addParent(linkedListNode)
	self:addParent(monsterData[inName]) 
	self:addParent(battleMonster)
	self:addParent(entity)
	self:addParent(battleBonus)

	log:print(log.LOG_DEBUG, "self.next: %s", self.next)
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

	log:print(log.LOG_DEBUG, "self.next: %s", self.next)

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
	inMonster.parentGroup = false
	self:remove(inMonster)
	self.size = self.size - 1
end

----------------------------------------
-- monsterGroup:targetIterator()
--
-- Since spell targetting picks a 
-- random member of the group, it is 
-- possible for a multi-target spell
-- to not hit every monster. This
-- function returns the head of the list
-- self.size times to make sure that the
-- spell is executed the correct number
-- of times.
----------------------------------------
function monsterGroup:targetIterator()
	local n		= 0
	local fullSize	= self.size
	local function f(_)
		if (self == nil) then
			error("Bad self reference", 2)
		end
		while (n < fullSize) do
			n = n + 1
			return self.head
		end
		return
	end

	return f, nil, n
end

