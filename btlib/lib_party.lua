-- Base party instance
-- 
party		= linkedList:new("partyNext", "partyPrev")

party.size	= 0
party.detect	= nil
party.shield	= nil
party.light	= nil
party.compass	= nil
party.levitate	= nil

----------------------------------------
-- party:addMember()
----------------------------------------
function party:addMember(inMember)
	self:insertTail(inMember)
	self:sort()
	self.size = self.size + 1
end

----------------------------------------
-- party:removeMember()
----------------------------------------
function party:removeMember(inMember)
	local l_member = false

	if (type(inMember) == "number") then
		l_member = self[inMember]
	elseif (type(inMember) == "string") then
		l_member = self:findByName(inMember)
	else
		l_member = inMember
	end

	self:remove(l_member)
	self.size = self.size - 1

	self:updateIndices()

	return l_member
end

----------------------------------------
-- party:updateIndices()
----------------------------------------
function party:updateIndices()
	local l_index
	local l_node

	for l_index = 1,7 do
		self[l_index] = false
	end

	l_index	= 1
	for l_node in self:iterator() do
		if ((self.indexHelper) and 
		    (type(self.indexHelper) == "function")) then
			self:indexHelper(l_node, l_index)
		end
		self[l_index] = l_node
		l_index = l_index + 1
	end
end

----------------------------------------
-- party:isOccupied()
----------------------------------------
function party:isOccupied(inSlot)
	if (type(inSlot) ~= "number") then
		inSlot = tonumber(inSlot)
	end

	return self[inSlot]
end

----------------------------------------
-- party:isEmpty()
----------------------------------------
function party:isEmpty()
	return (self.size == 0)
end

----------------------------------------
-- party:findByName()
--   Assumes that the node has a name
-- object
----------------------------------------
function party:findByName(inName)
	local l_node

	for l_node in self:iterator() do
		if (l_node.name == inName) then
			return l_node
		end
	end

	return false
end

----------------------------------------
-- party:toTable()
----------------------------------------
function party:toTable()
	local t	= {}
	local c

	for c in self:iterator() do
		table.insert(t, c.name)
	end

	return t
end

----------------------------------------
-- Stub Functions
--
-- These are standard functions that
-- need to be overloaded by individual
-- implementations
----------------------------------------

----------------------------------------
-- party:display()
----------------------------------------
function party:display()
	return false
end

----------------------------------------
-- party:sort()
--
-- Stub function. This is meant to be
-- overloaded by individual instances
----------------------------------------
function party:sort()
	self:updateIndices()
	self:display()
end

