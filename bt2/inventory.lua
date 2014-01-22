require "items"

----------------------------------------
-- inventorySlot class
----------------------------------------
local inventorySlot = {}
function inventorySlot:new(inName)
	local self = items:new(inName)

	self:addParent(inventorySlot)

	self.isEquipped		= false
	self.isIdentified	= false
	self.count		= false

	return self
end

function inventorySlot:canEquip(inClass)
	return self.can_equip[inClass]
end

function inventorySlot:equip() self.isEquipped = true end
function inventorySlot:unequip() self.isEquipped = false end
function inventorySlot:identify() self.isIdentified = true end

function inventorySlot:toInventoryLine(inCharacter)
	local rval = ""

	if (self.isEquipped) then
		rval = "*"
	elseif (not self:canEquip(inCharacter.class)) then
		rval = "@"
	else
		rval = " "
	end

	if (self.isIdentified) then
		rval = rval .. self.name
	else
		rval = rval .. self.type
	end

	return rval
end

----------------------------------------
-- inventory class
----------------------------------------
inventory = {}
function inventory:new()
	local self = array:new()

	self:addParent(inventory)

	self.maxSize = 8

	return self
end

function inventory:isEmpty()
	return (self.size == 0)
end

function inventory:isFull()
	if (self == nil) then
		error("Probably invalid inventory call", 2)
	end
	return (self.size == self.maxSize)
end

function inventory:findItem(inItem)
	local i

	for i = 1,self.size do
		if (self[i] == inItem) then
			return i
		end
	end

	return false
end

function inventory:dropItem(inSlot)
	if (type(inSlot) == "string") then
		local i

		for i = 1,self.size do
			if (self[i].name == inSlot) then
				self:remove(i)
				return
			end
		end
	elseif (type(inSlot == "number")) then
		self:remove(inSlot)
	end
end

function inventory:equipItem(inSlot)
	assert(self[inSlot] ~= nil)

	self[inSlot].isEquipped = true
end

function inventory:unequipItem(inSlot)
	self[inSlot].isEquipped = false
end

function inventory:printScreen(inCharacter)
	local i

	for i = 1,self.maxSize do
		text:print("\n%d)", i)

		if (self[i]) then
			text:print(self[i]:toInventoryLine(inCharacter))
		end
	end
end

function inventory:addItem(inName, isIdentified, inCount)
	assert(type(inName) == "string", 
				"addItem() passed a non-string")
	if (self:isFull()) then
		return false
	end

	local i

	i = inventorySlot:new(inName)
	i.isIdentified = isIdentified
	i.count = inCount or 1
	self:add(i)

	return true
end

function inventory:fromTable(inTable)
	local i

	log:print(log.LOG_DEBUG, "#inTable: %d", #inTable)
	for i = 1,#inTable do
		if (inTable[i].name ~= "") then
			local newSlot = inventorySlot:new(inTable[i].name)
			newSlot.isEquipped = inTable[i].isEquipped
			newSlot.isIdentified = inTable[i].isIdentified
			self:add(newSlot)
		end
	end
end

function inventory:toTable()
	local slot
	local rval = {}

	for i,slot in self:ipairs() do
		local t = {}
		t.name = slot.name
		t.isEquipped = slot.isEquipped
		t.isIdentified = slot.isIdentified
		t.count = slot.count
		table.insert(rval, t)

	end

	return rval
end
