require "random"

----------------------------------------
-- Table to hold all objects
----------------------------------------
local objectData = {}

----------------------------------------
-- object class
--
-- This is the base class for all BT
-- classes
----------------------------------------

object = {}
function object:new(inTable)
	local self = inTable or {}

	self.__parents	= {}
	table.insert(self.__parents, object)

	while true do
		self.key = random:hash()
		if (not objectData[self.key]) then
			objectData[self.key] = true
			break
		end
	end

	log:print(log.LOG_INFO, "object(%s) created from %s",
			self.key,
			log:getLocation(3)
		)

	setmetatable(self, {
		__index = object.index,
		__gc	= object.gc
		}
	)

	return self
end

----------------------------------------
-- object:addParent()
----------------------------------------
function object:addParent(...)
	local i

	for i = 1,select('#', ...) do
		local e = select(i, ...)
		if (type(e) ~= "table") then
			error("Trying to add a non-table parent", 2)
		end

		-- Only add the parent if it is not the object. This avoids
		-- a recursion error
		--
		if (e ~= self) then
			table.insert(self.__parents, e)
		end
	end
end

----------------------------------------
-- object:searchParents()
----------------------------------------
function object:searchParents(inKey)
	local i
	local c

	for i,c in ipairs(self.__parents) do
		if ((type(c) ~= "table") and
		    (type(c) ~= "userdata")) then
			error("Invalid class", 4)
		end

		local v = c[inKey]
		if (v ~= nil) then return v end
	end

	return nil
end

----------------------------------------
-- object:index()
----------------------------------------
function object:index(inKey)
	local v

	v = rawget(self, inKey)
	if (v) then
		return v
	end

	v = object.searchParents(self, inKey)

	if (type(v) == "table") then
		rawset(self, inKey, table.copy(v))
		return rawget(self, inKey)
	else
		if (inKey == nil) then
			error("table index is nil", 2)
		end
		self[inKey] = v
		return v
	end
end

----------------------------------------
-- object:gc()
----------------------------------------
function object:gc()
	log:print(log.LOG_INFO, "object(%s) cleared", self.key)
	objectData[self.key] = nil
end

----------------------------------------
-- table.copy()
--
-- Recursive copy of a table
----------------------------------------
function table.copy(inSource, inDestination)
	local t = inDestination or {}
	local k
	local v

	if (inSource == nil) then
		return nil
	end

	for k,v in pairs(inSource) do
		if (type(v) == "table") then
			rawset(t, k, table.copy(v))
		elseif (type(v) == "function") then
			local doNothing = 1
		else
			rawset(t, k, v)
		end
	end

	return t
end

----------------------------------------
-- table.setDefault()
----------------------------------------
function table.setDefault(inTable, inValue)
	setmetatable(inTable, {
		__index = function(t, k)
			local val

			val = rawget(t, k)
			if (val == nil) then
				return inValue
			end

			return val
		end
		}
	)
end
