require "dataio"
require "spells"

local _items = read_table("items")

items = {}
function items:new(inName)
	local self = {
	}

	btTable.addParent(self, _items[inName], items)
	btTable.setClassMetatable(self)

	self.__index = self

	return self
end

function items.getItemValueByName(name)
	assert(_items[name] ~= nil, "Invalid item name: " .. name)

	return _items[name].value
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
