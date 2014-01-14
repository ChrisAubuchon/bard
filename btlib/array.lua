require "object"

----------------------------------------
-- array class
--
-- This class can be used as a parent
-- via addParent(array) or as a new
-- object bia array:new()
----------------------------------------
array = {
	size = 0
}

function array:new()
	local self = object:new()

	self:addParent(array)

	return self
end

----------------------------------------
-- array:add()
----------------------------------------
function array:add(inValue)
	table.insert(self, inValue)
	self.size = #self
end

----------------------------------------
-- array:remove()
----------------------------------------
function array:remove(inIndex)
	table.remove(self, inIndex)
	self.size = #self
end

----------------------------------------
-- array:truncate()
----------------------------------------
function array:truncate(inIndex)
	local i

	for i = self.size, inIndex, -1 do
		table.remove(self, i)
	end
end

----------------------------------------
-- array:reverseIterator()
----------------------------------------
function array:reverseIterator()
	local n = self.size + 1

	local function f(_)
		if (self == nil) then
			error("Bad self reference", 2)
		end

		while (n > 1) do
			n = n - 1
			return self[n]
		end
		return
	end

	return f, nil, n
end

----------------------------------------
-- array:iterator()
----------------------------------------
function array:iterator()
	local n = 0
	local function f(_)
		if (self == nil) then
			error("Bad self reference", 2)
		end
		while (n < #self) do
			n = n + 1
			return self[n]
		end
		return
	end

	return f, nil, n
end

----------------------------------------
-- array:ipairs()
----------------------------------------
function array:ipairs()
	return ipairs(self)
end

----------------------------------------
-- array:find()
----------------------------------------
function array:find(inValue)
	local index
	local value

	for index, value in self:ipairs() do
		if (value == inValue) then
			return index
		end
	end

	return false
end
