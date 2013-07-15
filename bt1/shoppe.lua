local shoppe = {}
function shoppe:new(inTitle, inPic, inGreeting, inFileName)
	local self = {
		title		= inTitle,
		pic		= inPic,
		greeting	= inGreeting,
		fileName	= inFileName,
		inventory	= read_table(inFileName)
	}

	btTable.addParent(self, shoppe)
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
		write_table(self.inventory, inFileName)
	else
		write_table(self.inventory, self.fileName)
	end
end

function shoppe:printItem(inItem)
	text:print(inItem)
	text:setColumn(12)
	text:print("%7d", items.getItemValueByName(inItem))
end

function shoppe:setBigpic()
	bigpic:drawImage(self.pic)
	bigpic:setTitle(self.title)
end


----------------------------------------
-- enter()
----------------------------------------
function shoppe:enter()
	local inkey

	self:setBigpic()
	repeat
		text:clear()
		text:print(self.greeting)
		text:print("\nWhich of you is interested in my fine wares?")
		text:printExit()
		inkey = getkey()
		if ((inkey > "0") and (inkey < "7")) then
			inkey = tonumber(inkey)
			self:selectOption(party[inkey])
		end
	until (inkey == "E")
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
		text:print("\nBuy an item.")
		text:print("\nSell an item.")
		text:print("\nIdentify an item.")
		text:print("\nPool gold.")
		text:print("\nDone.")
		text:print("\n\nYou have %d gold.", inChar.gold)

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
			inkey = tonumber(inkey)
			if (party:isOccupied(inkey)) then
				party[inkey]:printCharacter()
				self:setBigpic()
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
		text:splashMessage("\nThat item is known already.")
		return
	end

	value = math.floor(item.value / 2)
	if (value > inChar.gold) then
		text:splashMessage("\nNot enough gold.")
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
