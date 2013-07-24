linkedList = {
	head	= false,
	tail	= false
}

linkedListNode = {
	prev	= false,
	next	= false
}

function linkedListNode:compare(inValue)
	return false
end

function linkedListNode:isLast()
	if (self.next) then
		return false
	else
		return true
	end
end

function linkedList:insertTail(inNode)
	if (not self.head) then
		self.head = inNode
		self.tail = inNode
		return
	end

	self.tail.next = inNode
	inNode.prev = self.tail
	inNode.next = false
	self.tail = inNode
end

function linkedList:insertHead(inNode)
	if (not self.head) then
		self.head = inNode
		self.tail = inNode
		return
	end

	inNode.prev = false
	inNode.next = self.head
	self.head.prev = inNode
	self.head = inNode
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
	inNode.next = inLocation
	inNode.prev = inLocation.prev
	inLocation.prev.next = inNode
	inLocation.prev = inNode
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

	inNode.prev = inLocation
	inNode.next = inLocation.next
	inLocation.next.prev = inNode
	inLocation.next = inNode
end

----------------------------------------
-- remove()
----------------------------------------
function linkedList:remove(inNode)
	if (inNode == self.head) then
		self.head = inNode.next
	end

	if (inNode == self.tail) then
		self.tail = inNode.prev
	end

	if (inNode.next) then
		inNode.next.prev = inNode.prev
	end

	if (inNode.prev) then
		inNode.prev.next = inNode.next
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
		if (head == nil) then
			if (not self.head) then
				return
			end

			return self.head
		end
		if (head.next) then
			return head.next
		end
		return
	end

	return f, nil, nil
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
		if (tail.prev) then
			return tail.prev
		end
		return
	end

	return f, nil, nil
end

----------------------------------------
-- ipairs()
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
		if (head.next) then
			head = head.next
			return n, head
		end
		return
	end
	return f, nil, 0
end
