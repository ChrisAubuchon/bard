monsterParty = {}
function monsterParty:new()
	local self = {
		size		= 0
	}

	btTable.addParent(self, monsterParty, battleBonus, linkedList)
	btTable.setClassMetatable(self)

	self.__index = self

	return self
end

----------------------------------------
-- printGroups()
----------------------------------------
function monsterParty:printGroups()
	local node
	local first	= true

	for node in self:iterator() do
		if (not first) then
			if (not node.next) then
				text:print(", and ")
			else
				text:print(", ")
			end
		else
			first = false
		end

		text:print("%d %s", node.size,
			pluralize(node.size, node.singular, node.plural)
			)
	end
	text:print(".\n\n")
end

----------------------------------------
-- addMonsterGroup()
----------------------------------------
function monsterParty:addMonsterGroup(inGroup)
	self.size = self.size + 1
	dprint(inGroup)
	self:insertTail(inGroup)
	dprint(self.head)
	dprint(self.tail)
end

----------------------------------------
-- removeMonsterGroup()
----------------------------------------
function monsterParty:removeMonsterGroup(inGroup)
	self.size = self.size - 1
	inGroup:deleteKey()
	self:remove(inGroup)
end

----------------------------------------
-- adjustMeleeGroups()
----------------------------------------
function monsterParty:adjustMeleeGroups()
	local i		= 1
	local mgroup

	for mgroup in self:iterator() do
		if (i < 3) then
			mgroup.inMeleeRange = true
		else
			mgroup.inMeleeRange = false
		end
	end
end

----------------------------------------
-- getLeadGroup()
----------------------------------------
function monsterParty:getLeadGroup()
	return self.head
end

----------------------------------------
-- disbelieve()
----------------------------------------
function monsterParty:doDisbelieve(inBattle)
	if (party.summon and party.summon.isIllusion and self.doDisbelieve) then
		local action = btAction:new()
		action.source = party:getFirstCharacter()
		action.target = self.head
		if (not action:savingThrow()) then
			text:cdprint(false, true, "Your foes see through your illusion!\n\n")
		end
	end
end

----------------------------------------
-- advance()
----------------------------------------
function monsterParty:advance()
	local mgroup

	for mgroup in self:reverseIterator() do
		if (not mgroup.prev) then return end

		if (mgroup.advanceSpeed > mgroup.prev.advanceSpeed) then
			self:remove(mgroup)
			self:insertBefore(mgroup, mgroup.prev)

			text:print("The %s advance%s!\n\n",
				pluralize(mgroup.size, mgroup.singular, 
						       mgroup.plural),
				pluralize(mgroup.size, "", "s")
				)
		end
	end
end

----------------------------------------
-- isAlive()
----------------------------------------
function monsterParty:isAlive()
	return self.head
end

----------------------------------------
-- doSummon()
----------------------------------------
function monsterParty:doSummon(inSummon)
	local mgroup

	if (self.size >= 4) then
		text:printEllipsis()
		return
	end
	mgroup = monsterGroup:new(inSummon.type, 1)
	mgroup.isIllusion = inSummon.isIllusion or false
	self:addMonsterGroup(mgroup)
	text:print(" and a %s appears!\n\n", mgroup.singular)
	party:display()
	timer:delay(3)
end












