monsterParty = {}
function monsterParty:new()
	local self = linkedList:new()

	self:addParent(monsterParty)
	self:addParent(battleBonus)

	self.size = 0

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

		text:print("%d %s (%d')", node.size,
			string.pluralize(node.size, node.singular, node.plural),
			node.range
			)
	end
	text:print(".\n\n")
end

----------------------------------------
-- addMonsterGroup()
----------------------------------------
function monsterParty:addMonsterGroup(inGroup)
	log:print(log.LOG_DEBUG, "monsterParty:addMonsterGroup()")
	log:print(log.LOG_DEBUG, "inGroup.next = %s", inGroup.next)
	self.size = self.size + 1
	self:insertTail(inGroup)

	local mgroup
	for mgroup in self:iterator() do
		log:print(log.LOG_DEBUG, "mgroup: %s", mgroup)
		log:print(log.LOG_DEBUG, "mgroup.next: %s", mgroup.next)
	end
end

----------------------------------------
-- removeMonsterGroup()
----------------------------------------
function monsterParty:removeMonsterGroup(inGroup)
	self.size = self.size - 1
	self:remove(inGroup)
end

----------------------------------------
-- monsterParty:isInMeleeRange()
--
-- Returns true if there is a group
-- with a range of 10
----------------------------------------
function monsterParty:isInMeleeRange()
	local mgroup

	local function inRangeFunction(m) return (m.range == 10) end

	for mgroup in self:conditionalIterator(inRangeFunction) do
		return true
	end

	return false
end

----------------------------------------
-- getLeadGroup()
----------------------------------------
function monsterParty:getLeadGroup()
	return self.head
end

----------------------------------------
-- getNearestGroup()
----------------------------------------
function monsterParty:getNearestGroup()
	local rgroup = false
	local mgroup

	for mgroup in self:iterator() do
		if (not rgroup) or (rgroup.range > mgroup.range) then
			rgroup = mgroup
		end
	end

	return rgroup
end

----------------------------------------
-- disbelieve()
----------------------------------------
function monsterParty:doDisbelieve()
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
				string.pluralize(mgroup.size, mgroup.singular, 
						       mgroup.plural),
				string.pluralize(mgroup.size, "", "s")
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
	party:display()
	text:ctdprint(false, true, " and a %s appears!\n\n", mgroup.singular)
end












