monster = {}

local _monsters
_monsters = diskio:readTable("monsters")

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

	self.maxHP = self.hpBase + random:xdy(self.hpRndNDice, self.hpRndDie)
	self.curHP = self.maxHP
	self.ac		= self.baseAC

	self.__index = self

	return self
end

----------------------------------------
-- monster:advanceGroup()
----------------------------------------
function monster:advanceGroup()
	local parent	= self.parentGroup
	local m

	parent.range = parent.range - self.advanceSpeed
	if (parent.range < 10) then
		parent.range = 10
	end

	text:print("The %s advance%s!!\n\n",
		string.pluralize(parent.size, self.singular, self.plural),
		string.pluralize(parent.size, "", "s")
		)

	-- Remove the attack priority for the other monsters in the group
	--
	for m in parent:iterator() do
		currentBattle:removePriority(m)
	end
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
	return random:betweenInclusive(self.priorityLo, self.priorityHi)
end

function monster:getPronoun()
	return self.pronoun
end

function monster:inMeleeRange()
	dprint(self.parentGroup.range)
	return (self.parentGroup.range == 10)
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
			index = random:xdy(1,4)
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

function monster:getTargetString()
	return string.format("a %s", self.singular)
end

function monster.getXPReward(inType)
	assert(_monsters[inType])

	return _monsters[inType].reward
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