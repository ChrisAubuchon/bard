garths = {}
local funcs = {}
local garthInventory = read_table("garthinv")

function funcs.selectInventory(player)
	local i
	local itemNumber
	local max

	if (player.inventory:isEmpty()) then
		text_cdprint(true, true, "\nYou have no items.")
		return false
	end

	text_clear()
	text_print("\nWhich item:")

	for i in player.inventory:iterator() do
		text_print("\n" .. i:toInventoryLine(player))
		text_setColumn(12)
		text_print("%7d", i.value / 2)
	end

	printCancel()

	itemNumber = text_select(2, player.inventory.size)
	if (not itemNumber) then
		return false
	end

	return itemNumber
end

function funcs.printGarthItem(itemName)
	text_print(itemName)
	text_setColumn(12)
	text_print("%7d", items.getItemValueByName(itemName))
end

function funcs.buyItem(player)
	local itemList = {}
	local item
	local itemValue

	if (player.inventory:isFull()) then
		text_cdprint(true, true, "\nYour pockets are full.")
		return
	end

	while true do
		for itemNumber,item in ipairs(garthInventory) do
			if (garthInventory[item].count ~= 0) then
				table.insert(itemList, item)
			end
		end

		item = text_scrollSelect(itemList, funcs.printGarthItem)
		if (not item) then
			return	
		end

		itemValue = items.getItemValueByName(item)
		if (itemValue > player.gold) then
			text_cdprint(true, true, "\nNot enough gold.")
			return
		end

		if (garthInventory[item].count ~= -1) then
			garthInventory[item].count = garthInventory[item].count - 1
		end

		player:giveItem(item, true)
		player.gold = player.gold - itemValue
		text_clearline(11)
		text_setCursor(0, 11)
		text_cdprint(false, true, "Done!        ")

		if (player.inventory:isFull()) then
			return
		end
	end
	
end

function funcs.identifyItem(player)
	local itemNumber
	local inv
	local value

	itemNumber = funcs.selectInventory(player)
	if (not itemNumber) then
		return
	end

	inv = player.inventory[itemNumber]
	if (inv.isIdentified) then
		splashMessage("\nThat item is known already.")
		return
	end

	value = math.floor(inv.value / 2)
	if (value > player.gold) then
		splashMessage("\nNot enough gold.")
		return
	end

	player.gold = player.gold - value
	inv.identify()
end

function funcs.sellItem(player)
	local itemNumber
	local inv
	local value
	local itemName

	itemNumber = funcs.selectInventory(player)
	if (not itemNumber) then
		return
	end

	inv = player.inventory[itemNumber]
	itemName = inv.name

	if (garthInventory[itemName].count ~= -1) then
		garthInventory[itemName].count = garthInventory[itemName].count + 1
	end

	player.gold = player.gold + (math.floor(inv.value / 2))
	player.inventory:dropItem(itemNumber)
	player.calcAC()
end

function funcs.selectOption(player)
	local inkey

	repeat
		text_clear()
		text_print("Greetings, "..player.name)
		text_print(". Would you like to:")
		text_setCursor(0, 3)
		text_print("\nBuy an item.")
		text_print("\nSell an item.")
		text_print("\nIdentify an item.")
		text_print("\nPool gold.")
		text_print("\nDone.")
		text_print("\n\nYou have: " .. tostring(player.gold) .. " gold.")
		inkey = getkey()
		if (inkey == "B") then
			funcs.buyItem(player)
		elseif (inkey == "I") then
			funcs.identifyItem(player)
		elseif (inkey == "P") then
			character.poolGold(player)
		elseif (inkey == "S") then
			funcs.sellItem(player)
		elseif ((inkey > "0") and (inkey < "7")) then
			inkey = tonumber(inkey)
			if (party:isOccupied(inkey)) then
				party[inkey]:printCharacter()
				bigpic.drawBigpic("PIC_GARTHINT")
				bigpic.setTitle("The Shoppe")
			end
		end
	until (inkey == "D")
end

function funcs.selectCharacter()
	local inkey

	repeat
		text_clear()
		text_print("Welcome to Garth's Equipment Shoppe, oh wealthy travelers!")
		text_print("\nWhich of you is interested in my fine wares?")
		printExit()
		inkey = getkey()
		if ((inkey > "0") and (inkey < "7")) then
			inkey = tonumber(inkey)
			if (party:isOccupied(inkey)) then
				funcs.selectOption(party[inkey])
			end
		end
	until (inkey == "E")
end

function garths.enter()
	bigpic.drawBigpic("PIC_GARTHINT")
	bigpic.setTitle("The Shoppe")
	funcs.selectCharacter()
end
