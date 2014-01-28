require "object"
require "random"

----------------------------------------
-- linkedListNode class
----------------------------------------
linkedListNode = {
	prev	= false,
	next	= false
}

function linkedListNode:new()
	local self = object:new()

	self:addParent(linkedListNode)

	return self
end

----------------------------------------
-- linkedListNode:compare()
----------------------------------------
function linkedListNode:compare(inValue)
	return false
end

----------------------------------------
-- linkedListNode:isLast()
----------------------------------------
function linkedListNode:isLast()
	if (self[self.next]) then
		return false
	else
		return true
	end
end

----------------------------------------
-- linkedList class
--
-- This class can be used as a parent
-- via addParent(linkedList) or as a
-- new object bia linkedList:new()
----------------------------------------
linkedList = {
	head	= false,
	tail	= false
}

function linkedList:new(inNext, inPrev, inCompare)
	local self = object:new()

	self:addParent(linkedList)

	self.head	= false
	self.tail	= false
	self.next	= inNext or "next"
	self.prev	= inPrev or "prev"
	self.compare	= inCompare or nil

	return self
end

----------------------------------------
-- getFirst()
----------------------------------------
function linkedList:getFirst()
	return self.head
end

----------------------------------------
-- getLast()
----------------------------------------
function linkedList:getLast()
	return self.tail
end

----------------------------------------
-- insertTail()
----------------------------------------
function linkedList:insertTail(inNode)
	if (not self.head) then
		self.head = inNode
		self.tail = inNode
		return
	end

	self.tail[self.next]	= inNode
	inNode[self.prev]	= self.tail
	inNode[self.next]	= false
	self.tail		= inNode
end

----------------------------------------
-- insertHead()
----------------------------------------
function linkedList:insertHead(inNode)
	if (not self.head) then
		self.head = inNode
		self.tail = inNode
		return
	end

	inNode[self.prev]	= false
	inNode[self.next]	= self.head
	self.head[self.prev]	= inNode
	self.head		= inNode
end

----------------------------------------
-- insertBefore()
--
-- Add inNode to the linked list before
-- inLocation
----------------------------------------
function linkedList:insertBefore(inNode, inLocation)
	assert(inLocation, "Trying to insertBefore a non-existent location")

	-- Call insertHead if inserting at the beginning
	--
	if (inLocation == self.head) then
		self:insertHead(inNode)
		return
	end

	-- The assumption now is that inLocation has a 
	-- prev entry.
	--
	inNode[self.next]			= inLocation
	inNode[self.prev]			= inLocation[self.prev]
	inLocation[self.prev][self.next]	= inNode
	inLocation[self.prev]			= inNode
end

----------------------------------------
-- insertAfter()
--
-- Add inNode to the linked list after
-- inLocation
----------------------------------------
function linkedList:insertAfter(inNode, inLocation)
	assert(inLocation, "Trying to insertAfter a non-existent location")

	if (inLocation == self.tail) then
		self:insertTail(inNode)
		return
	end

	inNode[self.prev]			= inLocation
	inNode[self.next]			= inLocation[self.next]
	inLocation[self.next][self.prev]	= inNode
	inLocation[self.next]			= inNode
end

----------------------------------------
-- remove()
----------------------------------------
function linkedList:remove(inNode)
	if (inNode == self.head) then
		self.head = inNode[self.next]
	end

	if (inNode == self.tail) then
		self.tail = inNode[self.prev]
	end

	if (inNode[self.next]) then
		inNode[self.next][self.prev] = inNode[self.prev]
	end

	if (inNode[self.prev]) then
		inNode[self.prev][self.next] = inNode[self.next]
	end
end

----------------------------------------
-- removeHead()
----------------------------------------
function linkedList:removeHead()
	self.head = self.head[self.next]
	self.head[self.prev] = false
end

----------------------------------------
-- removeTail()
----------------------------------------
function linkedList:removeTail()
	if (self.tail) then
		self.tail = self.tail[self.prev]
		self.tail[self.next] = false
	end
end

----------------------------------------
-- find()
--
-- Find a node that matches inValue.
-- It calls a :compare method to
-- detemine a match
----------------------------------------
function linkedList:find(inValue)
	local node

	for node in self:iterator() do
		if (node:compare(inValue)) then
			return node
		end
	end

	return false
end

----------------------------------------
-- iterator()
----------------------------------------
function linkedList:iterator()
	local function f(_, head)
		if (not self.head) then return end
		if (head == nil) then
			return self.head
		end
		if (head[self.next]) then
			return head[self.next]
		end
		return
	end

	return f, nil, nil
end

----------------------------------------
-- iterateFrom()
----------------------------------------
function linkedList:iterateFrom(inHead, isCircular)
	local function f(_, head)
		if (not self.head) then return end

		if (head == nil) then
			return inHead
		end
		if (isCircular and (head[self.next] == inHead)) then
			return
		end
		if (head[self.next]) then
			return head[self.next]
		end
		if (isCircular and (head ~= self.head)) then
			
			return self.head
		end
			
		return
	end

	return f, nil, nil
end

----------------------------------------
-- conditionalIterator()
----------------------------------------
function linkedList:conditionalIterator(inConditionFunction)
	local state = self.head
	local function f(_)
		while (state) do
			if (inConditionFunction(state)) then
				local rval = state
				state = state[self.next]
				return rval
			end
			state = state[self.next]
		end
		return
	end
	return f,nil,state
end

----------------------------------------
-- conditionalIterateFrom()
----------------------------------------
function linkedList:conditionalIterateFrom(inHead, inConditionFunction)
	local state	= inHead
	local function f(_)
		while (state) do
			if (inConditionFunction(state)) then
				local rval = state
				state = state[self.next]
				return rval
			end
			state = state[self.next]
		end
		return
	end
	return f,nil,state
end

----------------------------------------
-- reverseIterator()
----------------------------------------
function linkedList:reverseIterator()
	local function f(_, tail)
		if (tail == nil) then
			if (not self.tail) then
				return
			end

			return self.tail
		end
		if (tail[self.prev]) then
			return tail[self.prev]
		end
		return
	end

	return f, nil, nil
end

----------------------------------------
-- linkedList:ipairs()
--
-- Return an index,value iterator
----------------------------------------
function linkedList:ipairs()
	local head = nil
	local function f(_, n)
		n = n + 1
		if (head == nil) then
			if (not self.head) then
				return
			end

			head = self.head
			return n, head
		end
		if (head[self.next]) then
			head = head[self.next]
			return n, head
		end
		return
	end
	return f, nil, 0
end


----------------------------------------
-- linkedList:random()
--
-- Return a random node 
----------------------------------------
function linkedList:randomMember()
	local node
	local rval		= false
	local numerator		= 1
	local denominator	= 2
	local comparator

	for node in self:iterator() do
		if (not rval) then
			rval = node
		else
			comparator = (numerator / denominator) * 100000
			if ((random:rnd() % 100000) > comparator) then
				rval = node
			end
			numerator = numerator + 1
			denominator = denominator + 1
		end
	end

	return rval
end

----------------------------------------
-- linkedList:truncate()
--
-- Remove all elements after inNode
----------------------------------------
function linkedList:truncate(inNode)
	local node 	= inNode or self.head

	while (node[self.next]) do
		self:remove(node[self.next])
	end
end
