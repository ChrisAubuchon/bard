btTable = {}

local function classSearch(inKey, inClassList)
	local c

	for _,c in ipairs(inClassList) do
		if ((type(c) ~= "table") and
		    (type(c) ~= "userdata")) then
			dprint(type(c))
			error("Invalid class", 4)
		end
		local v = c[inKey]
		if (v ~= nil) then return v end
	end

	return nil
end

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
		else
			rawset(t, k, v)
		end
	end

	return t
end

function table.print(inTable)
	local k
	local v

	for k,v in pairs(inTable) do
		if (type(v) == "table") then
			print(string.format("%s: {", k))
			table.print(v)
			print("}")
		else
			print(string.format("%s: %s", k, v))
		end
	end
end


function btTable.addParent(inSelf, ...)
	local i

	if (not inSelf.__parents) then
		inSelf.__parents = {}
	end

	for i = 1,select('#', ...) do
		local e = select(i, ...)
		if (type(e) ~= "table") then
			error("Trying to add a non-table parent", 2)
		end
		table.insert(inSelf.__parents, e)
	end
end

function btTable.setClassMetatable(inSelf)
	setmetatable(inSelf, {
		__index = function(t,k)
			local v = classSearch(k, inSelf.__parents)
			if (type(v) == "table") then
				rawset(t, k, table.copy(v))
				return rawget(t, k)
			else
				t[k] = v
				return v
			end
		end
		}
	)
end

local function __false_mt(t, key)
	local val

	val = rawget(t, key)
	if (val == nil) then
		return false,false
	end

	return val,true
end

local function __zero_mt(t, key)
	local val

	val = rawget(t, key)
	if (val == nil) then
		return 0,false
	end

	return val,true
end

btTable.new = function (useZeroTable)
	local self = {
	}

	if (useZeroTable) then
		setmetatable(self, { __index = __zero_mt })
	else
		setmetatable(self, { __index = __false_mt })
	end

	return self
end

btDefaultTable = {}
function btDefaultTable:new(defaultValue)
	local self = {}

	local function mt(t, key)
		local val

		val = rawget(t, key)
		if (val == nil) then
			return defaultValue
		end

		return val
	end

	setmetatable(self, { __index = mt })

	return self
end

btArray = {
	size = 0
}

function btArray:__add(inValue)
	table.insert(self, inValue)
	self.size = #self
end

function btArray:__remove(inIndex)
	table.remove(self, inIndex)
	self.size = #self
end

function btArray:truncate(inPos)
	local i

	for i = #self,inPos,-1 do
		table.remove(self, i)
	end
end

function btArray:reverseIterator()
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

function btArray:iterator()
	local n = 0
	local function f(_)
		if (self == nil) then
			error("Bad self reference", 2)
		end
		while (n < self.size) do
			n = n + 1
			return self[n]
		end
		return
	end

	return f, nil, n
end

function btArray:ipairs()
	return ipairs(self)
end
