local shoppe = {}
function shoppe:new(inTitle, inPic, inGreeting, inFileName)
	local self = {
		greeting	= inGreeting,
		fileName	= inFileName,
		inventory	= diskio:readTable(inFileName)
	}

	btTable.addParent(self, shoppe, building:new(inTitle, inPic))
	btTable.setClassMetatable(self)

	return self
end

function shoppe:toArray()
	local t = {}
	local item

	for _,item in ipairs(self.inventory) do
		if (self.inventory[item].count ~= 0) then
			table.insert(t, item)
		end
	end

	return t
end

function shoppe:writeInventory(inFileName)
	if (inFileName) then
		diskio:writeTable(self.inventory, inFileName)
	else
		diskio:writeTable(self.inventory, self.fileName)
	end
end

function shoppe:printItem(inItem)
	text:print(inItem)
	text:setColumn(12)
	text:print("%7d", items.getItemValueByName(inItem))
end

----------------------------------------
-- enter()
----------------------------------------
function shoppe:enter()
	local inkey
	local c

	if (globals.isNight) then
		text:clear()
		text:print("\nThe shoppe is closed at night.")
		text:printContinue()
		getkey()
		text:clear()
		currentLevel:turnParty("back")
		return
	end

	self:resetBigpic()
	repeat
		text:clear()
		text:print(self.greeting)
		text:print("\nWhich of you is interested in my fine wares?")
		text:printExit()
		inkey = getkey()
		c = party:readMember(inkey)
		if (c) then
			self:selectOption(c)
		end
	until (inkey == "E")

	text:clear()
	currentLevel:turnParty("back")
end

----------------------------------------
-- selectOption()
----------------------------------------
function shoppe:selectOption(inChar)
	local inkey

	repeat
		text:clear()
		text:print("Greetings, %s. Would you like to:", inChar.name)
		text:setCursor(0, 3)
		text:print(
			"\nBuy an item." ..
			"\nSell an item." ..
			"\nIdentify an item." ..
			"\nPool gold." ..
			"\nDone." ..
			"\n\nYou have %d gold.",
			inChar.gold 
			)

		inkey = getkey()
		if (inkey == "B") then
			self:buyItem(inChar)
		elseif (inkey == "I") then
			self:identifyItem(inChar)
		elseif (inkey == "P") then
			inChar:poolGold()
		elseif (inkey == "S") then
			self:sellItem(inChar)
		elseif ((inkey > "0") and (inkey < "7")) then
			local c = party:readMember(inkey)
			if (c) then
				c:printCharacter()
				self:resetBigpic()
			end
		end
	until (inkey == "D")
end

----------------------------------------
-- buyItem()
----------------------------------------
function shoppe:buyItem(inChar)
	local item
	local itemValue

	if (inChar.inventory:isFull()) then
		text:cdprint(true, true, "\nYour pockets are full.")
		return
	end

	while true do
		item = text:scrollingSelect(self, self.printItem)
		if (not item) then	
			return
		end

		itemValue = items.getItemValueByName(item)
		if (itemValue > inChar.gold) then
			text:cdprint(true, true, "\nNot enough gold.")
			return
		end

		if (self.inventory[item].count ~= -1) then
			self.inventory[item].count = 
				self.inventory[item].count - 1
			self:writeInventory()
		end

		inChar:giveItem(item, true)
		inChar.gold = inChar.gold - itemValue
		text:clearLine(11)
		text:setCursor(0, 11)
		text:cdprint(false, true, "Done!        ")

		if (inChar.inventory:isFull()) then
			return
		end
	end
end

----------------------------------------
-- selectInventory()
----------------------------------------
function shoppe:selectInventory(inChar)
	local item
	local itemNumber
	local max

	if (inChar.inventory:isEmpty()) then
		text:cdprint(true, true, "\nYou have no items.")
		return false
	end

	text:clear()
	text:print("\nWhich item:")

	for item in inChar.inventory:iterator() do
		text:print("\n%s", item:toInventoryLine(inChar))
		text:setColumn(12)
		text:print("%7d", math.floor(item.value / 2))
	end

	text:printCancel()

	itemNumber = text:select(2, inChar.inventory.size)
	if (not itemNumber) then
		return false
	end

	return itemNumber
end

----------------------------------------
-- identifyItem()
----------------------------------------
function shoppe:identifyItem(inChar)
	local itemNumber
	local item
	local value

	itemNumber = self:selectInventory(inChar)
	if (not itemNumber) then
		return
	end

	item = inChar.inventory[itemNumber]
	if (item.isIdentified) then
		text:csplash(true, true, "\nThat item is known already.")
		return
	end

	value = math.floor(item.value / 2)
	if (value > inChar.gold) then
		text:csplash(true, true, "\nNot enough gold.")
		return
	end

	inChar.gold = inChar.gold - value
	item:identify()
end

----------------------------------------
-- sellItem()
----------------------------------------
function shoppe:sellItem(inChar)
	local item
	local itemNumber
	local itemValue

	itemNumber = self:selectInventory(inChar)
	if (not itemNumber) then
		return
	end

	item = inChar.inventory[itemNumber]
	if (self.inventory[item.name].count ~= -1) then
		self.inventory[item.name].count = 
			self.inventory[item.name].count + 1
		self:writeInventory()
	end

	inChar.gold = inChar.gold + (math.floor(item.value / 2))
	inChar.inventory:dropItem(itemNumber)
	party:display()
end


garths = shoppe:new(
	"The Shoppe", 
	"PIC_GARTHINT", 
	"Welcome to Garth's Equipment Shoppe, oh wealthy travelers!",
	"garthinv"
)
