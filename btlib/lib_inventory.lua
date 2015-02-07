-- Internal inventorySlot class
inventorySlot = {}


----------------------------------------
-- inventorySlot:new()
----------------------------------------
function inventorySlot:new(inName)
	local self

	if (not itemList) then
		log:print(log.LOG_ERROR, "Must be an itemList class")
		os.exit(1)
	end

	self	= itemList:getItem(inName)

	self:addParent(inventorySlot)

	self.isEquipped		= false
	self.isIdentified	= false
	self.contains		= false
	self.count		= false

	return self
end

----------------------------------------
-- inventory Class
----------------------------------------
inventory	= {}

----------------------------------------
-- inventory:new()
----------------------------------------
function inventory:new(inMaxSize)
	local self	= array:new()

	self:addParent(inventory)

	self.maxSize	= inMaxSize or 8

	return self
end

----------------------------------------
-- inventory:isEmpty()
----------------------------------------
function inventory:isEmpty()
	return (self.size == 0)
end

----------------------------------------
-- inventory:isFull()
----------------------------------------
function inventory:isFull()
	return (self.size >= self.maxSize)
end

----------------------------------------
-- inventory:fromTable()
----------------------------------------
function inventory:fromTable(inTable)
	local i

	for i = 1,#inTable do
		if (inTable[i].name ~= "") then
			local newSlot	= inventorySlot:new(inTable[i].name)
			newSlot.isEquipped	= inTable[i].isEquipped
			newSlot.isIdentified	= inTable[i].isIdentified
			newSlot.count		= inTable[i].count or 0
			newSlot.contains	= inTable[i].contains or false
			self:add(newSlot)
		end
	end
end

----------------------------------------
-- inventory:toTable()
----------------------------------------
function inventory:toTable()
	local i
	local slot
	local rval	= {}

	for i, slot in self:ipairs() do
		local t	= {}

		t.name		= slot.name
		t.isEquipped	= slot.isEquipped
		t.isIdentified	= slot.isIdentified
		t.count		= slot.count or false
		t.contains	= slot.contains or false

		table.insert(rval, t)
	end

	return rval
end

