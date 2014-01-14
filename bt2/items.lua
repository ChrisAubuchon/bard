require "spells"

local _items = diskio:readTable("items")

items = {}
function items:new(inName)
	local self = object:new()
	
	self:addParent(_items[inName])
	self:addParent(items)

	return self
end

function items.getItemValueByName(name)
	assert(_items[name] ~= nil, "Invalid item name: " .. name)

	return _items[name].value
end

function items.fromBTI(inName)
	local rval = {}

	assert(_items[inName])

	rval.name = inName
	if (_items[inName].max_charges) then
		rval.charges = _items[inName].max_charges
	end

	return rval
end

----------------------------------------
-- __init()
--
-- Item list initialization
----------------------------------------
local function __init()
	local i
	local func
	local w

	for _,i in pairs(_items) do
		if (i.action) then
			compileAction(i.action)
		end
	end
end

__init()
