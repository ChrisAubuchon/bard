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

function linkedListNode:delete()
	self.prev = nil
	self.next = nil
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
	if (self[self.__next]) then
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
	self.__next	= inNext or "next"
	self.__prev	= inPrev or "prev"
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

	self.tail[self.__next]	= inNode
	inNode[self.__prev]	= self.tail
	inNode[self.__next]	= false
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

	inNode[self.__prev]	= false
	inNode[self.__next]	= self.head
	self.head[self.__prev]	= inNode
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
	inNode[self.__next]			= inLocation
	inNode[self.__prev]			= inLocation[self.__prev]
	inLocation[self.__prev][self.__next]	= inNode
	inLocation[self.__prev]			= inNode
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

	inNode[self.__prev]			= inLocation
	inNode[self.__next]			= inLocation[self.__next]
	inLocation[self.__next][self.__prev]	= inNode
	inLocation[self.__next]			= inNode
end

----------------------------------------
-- remove()
----------------------------------------
function linkedList:remove(inNode)
	if (inNode == self.head) then
		self.head = inNode[self.__next]
	end

	if (inNode == self.tail) then
		self.tail = inNode[self.__prev]
	end

	if (inNode[self.__next]) then
		inNode[self.__next][self.__prev] = inNode[self.__prev]
	end

	if (inNode[self.__prev]) then
		inNode[self.__prev][self.__next] = inNode[self.__next]
	end
end

----------------------------------------
-- removeHead()
----------------------------------------
function linkedList:removeHead()
	self.head = self.head[self.__next]
	self.head[self.__prev] = false
end

----------------------------------------
-- removeTail()
----------------------------------------
function linkedList:removeTail()
	if (self.tail) then
		self.tail = self.tail[self.__prev]
		self.tail[self.__next] = false
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
if false then
	local state = self.head
	local function f(_, head)
		local rval
		if (not state) then
			return
		end

		rval = state
		state = state[self.__next]
		return rval
	end
end
	local function f(_, head)
		if (not self.head) then return end
		if (head == nil) then
			return self.head
		end
		if (head[self.__next]) then
			return head[self.__next]
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
		if (isCircular and (head[self.__next] == inHead)) then
			return
		end
		if (head[self.__next]) then
			return head[self.__next]
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
				state = state[self.__next]
				return rval
			end
			state = state[self.__next]
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
				state = state[self.__next]
				return rval
			end
			state = state[self.__next]
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
		if (tail[self.__prev]) then
			return tail[self.__prev]
		end
		return
	end

	return f, nil, nil
end

----------------------------------------
-- linkedList:reverseConditionalIterator()
----------------------------------------
function linkedList:reverseConditionalIterator(inConditionFunction)
	local state = self.tail
	local function f(_)
		while(state) do
			if (inConditionFunction(state)) then
				local rval = state
				state = state[self.__prev]
				return rval
			end
			state = state[self.__prev]
		end
		return
	end
	return f,nil,state
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
		if (head[self.__next]) then
			head = head[self.__next]
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

	while (node[self.__next]) do
		self:remove(node[self.__next])
	end
end

----------------------------------------
-- linkedList:delete()
----------------------------------------
function linkedList:delete()
	local node 
	local next	 = self.head

	while (next) do
		node = next
		next = next[self.__next]

		self:remove(node)
		if ((node.delete) and (type(node.delete) == "function")) then
			node:delete()
		end
	end

	self.head = false
	self.tail = false
end
