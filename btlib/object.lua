require "random"

local memoryTrace = true

----------------------------------------
-- Table to hold all objects
--
-- Mark the values as weak to not
-- interfere with garbage collection.
----------------------------------------
local objectData = {}
local objectDataMetaTable = { __mode = "v" }
setmetatable(objectData, objectDataMetaTable)

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
			objectData[self.key] = self
			break
		end
	end

	if (memoryTrace) then
		self.__location = log:getLocation(3)
	end

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
	if (memoryTrace) then
		log:print(log.LOG_MEMORY, 
			"object(%s) from \"%s\" garbage collected", 
			self.key,
			self.__location
		)
	end

	objectData[self.key] = nil
end

----------------------------------------
-- object:dumpAll()
----------------------------------------
function object:dumpAll()
	local key
	local value

	for key,value in pairs(objectData) do
		log:print(log.LOG_INFO, "Key: %s from %s",
			key,
			value.__location
			)
	end
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
