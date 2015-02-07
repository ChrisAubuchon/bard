camp = {}

----------------------------------------
-- camp:enter()
----------------------------------------
function camp:enter()
	local self	= object:new()
	local inkey
	local keys

	self:addParent(camp)
	self:addParent(building:new("The Ruin", "PIC_CAMP"))


--	XXX - Uncomment when ready
--	party:songTimeout()
--	party:disablePassiveEffects()

	self:resetBigpic()

	repeat
		keys	= {} 

		text:cdprint(true, false, 
			"Thou art in the Camp of Skara Brae.\n\n")

		if (party:hasEmptySlot()) then
			keys.A = true
			text:print("Add a member\n")
		end

		if (not party:isEmpty()) then
			text:print("Remove a member\n")
		else
			text:print("Rename a member\n")
		end

		text:print("Create a member\n" ..
				"Transfer characters\n" ..
				"Delete a member\n")

		if (not party:isEmpty()) then
			keys.S = true
--			keys.E = true
			text:print("Save the party\n")
--			text:print("Enter Wilderness")
		end

		text:print("Leave the game\n")

		inkey = getkey()

		if ((inkey == "A") and (keys["A"])) then
			self:addMember()
		elseif (inkey == "C") then	
			self:createMember()
		elseif (inkey == "R") then
			if (party:isEmpty()) then
				self:renameMember()
			else
				self:removeMember()
			end
		elseif ((inkey == "S") and (keys.S)) then
			self:saveParty()
		elseif ((inkey > "0") and (inkey < "8")) then
			local c = party:isOccupied(inkey)
			if (c) then
				c:print()
				self:resetBigpic()
			end
		end
	until (inkey == "L")

	globals.gameState = globals.STATE_EXIT
end

----------------------------------------
-- camp:addMember()
----------------------------------------
function camp:addMember()
	local member

	local function campHeader()
		text:print("Who shall join?\n")
	end

	text:clear()

	if (roster:countCharacters() == 0) then
		text:print("There are no saved characters.\n")
		getkey()
		return
	end

	member = text:scrollingSelect(roster, roster.printMember, 1, 8,
			campHeader, globals.scrollFooter)

	if (not member) then
		return
	end

	if (roster:isParty(member)) then
		party:addParty(roster:readParty(member))
	else
		if (party:findByName(member)) then
			text:cdprint(true, false, 
				"%s\n is already in the party.",
				member)
			getkey()
		else
			party:addMember(roster:readCharacter(member))
		end
	end
end

----------------------------------------
-- camp:createMember()
----------------------------------------
function camp:createMember()
	local newChar
	local inkey

	----------------------------------------
	-- getGender()
	----------------------------------------
	local function getGender()
		local inkey

		repeat
			text:cdprint(true, false, 
				"Do you wish your character to be\n" ..
				"Male or \nFemale?")

			inkey = getkey()

			if (inkey == "F") then
				return "Female"
			elseif (inkey == "M") then
				return "Male"
			end
		until (inkey == btkeys.ESCAPE)

		return false
	end

	----------------------------------------
	-- getRace()
	----------------------------------------
	local function getRace()
		local inkey

		repeat
			text:cdprint(true, false,
				"Select a race for your new character:\n\n" ..
				"1) Human\n" ..
				"2) Elf\n" ..
				"3) Dwarf\n" ..
				"4) Hobbit\n" ..
				"5) Half-Elf\n" ..
				"6) Half-Orc\n" ..
				"7) Gnome")

			inkey = getkey()
			if (inkey == "1") then
				return "Human"
			elseif (inkey == "2") then	
				return "Elf"
			elseif (inkey == "3") then	
				return "Dwarf"
			elseif (inkey == "4") then
				return "Hobbit"
			elseif (inkey == "5") then
				return "Half-Elf"
			elseif (inkey == "6") then
				return "Half-Orc"
			elseif (inkey == "7") then
				return "Gnome"
			end
		until (inkey == btkeys.ESCAPE)

		return false
	end

	----------------------------------------
	-- getStartingClass
	----------------------------------------
	local function getStartingClass(inChar)
		local i
		local sclass
		local sclassList
		local inkey

		text:cdprint(true, false, 
			"St:%2d IQ: %2d Dx: %2d Cn:%2d Lk: %2d\n",
			inChar.st, inChar.iq, inChar.dx, inChar.cn,
			inChar.lk)

		sclassList = race:getStartingClasses(inChar.race)

		for i,sclass in ipairs(sclassList) do
			text:print("%d) %s\n", i, sclass)
		end

		repeat 
			inkey = getkey()
			if ((inkey >= "1") and (inkey <= tostring(#sclassList))) then
				return sclassList[tonumber(inkey)]
			end
		until (inkey == btkeys.ESCAPE)

		return false
	end

	----------------------------------------
	-- getName()
	----------------------------------------
	local function getName()
		local name

		repeat
			name	= text:readString(14, true)
			if (not name) then
				return false
			end

			name	= name:gsub("*", "X")

			if (roster:nameExists(name)) then
				text:cdprint(true, false,
					"There is already a character " ..
					"with that name in the roster."
					)
			else
				return name
			end
		until false			
	end


	repeat
::restart::
		newChar		= {}
		newChar.spells	= {}
		newChar.songs	= {}

		newChar.gender = getGender()
		if (not newChar.gender) then
			return false
		end

		newChar.race	= getRace()
		if (not newChar.race) then
			return false
		end

		newChar.st	= race:randomStat(newChar.race, "base_st")
		newChar.dx	= race:randomStat(newChar.race, "base_dx")
		newChar.iq	= race:randomStat(newChar.race, "base_iq")
		newChar.cn	= race:randomStat(newChar.race, "base_cn")
		newChar.lk	= race:randomStat(newChar.race, "base_lk")

		newChar.maxHp	= random:band(15) + 13
		newChar.currentHp	= newChar.maxHp

		newChar.class	= getStartingClass(newChar)
		if (not newChar.class) then
			goto restart
		end

		if (newChar.gender == "Male") then
			newChar.pic	= "PIC_MALE" .. 
					  classes:getPic(newChar.class)
		else
			newChar.pic	= "PIC_FEMALE" .. 
					  classes:getPic(newChar.class)
		end

		classes:getStartingAbilities(newChar)
		newChar.inventory = classes:getStartingInventory(newChar)

		newChar.cur_level	= 1
		newChar.max_level	= 1

		if (classes:isSpellCaster(newChar.class)) then
			newChar.currentSppt	= random:band(15) + 8
			newChar.maxSppt		= newChar.currentSppt
		else
			newChar.currentSppt	= 0
			newChar.maxSppt		= 0
		end

		text:cdprint(true, false, "Name your new character --")
		newChar.name	= getName()
		if (not newChar.name) then
			return false
		end

		roster:writeCharacter(newChar, true)
		return
	until (inkey == btkeys.ESCAPE)
end

----------------------------------------
-- camp:removeMember()
----------------------------------------
function camp:removeMember()
	local memberList	= object:new()
	local member

	local function remove_toArray()
		local rval = {}
		local i

		table.insert(rval, "Remove them all!")
		for i in party:iterator() do
			table.insert(rval, i.name)
		end

		return rval
	end

	local function removeHeader()
		text:print("Select which party member to remove or...\n")
	end

	local function remove_print(_, inName)
		text:print(inName)
	end

	memberList.toArray = remove_toArray

	member = text:scrollingSelect(memberList, remove_print, 3, 8,
			removeHeader, globals.scrollFooter)

	if (not member) then
		return
	end

	if (member == "Remove them all!") then
		local m

		for m in party:iterator() do
			party:removeMember(m)

			roster:writeCharacter(m)
		end
	else
		member = party:removeMember(member)

		roster:writeCharacter(member)
	end

	party:display()
end

----------------------------------------
-- camp:renameMember()
----------------------------------------
function camp:renameMember()
	local oldMember
	local newMember

	local function renameHeader()
		text:print("Rename who?\n")
	end

	text:clear()

	if (roster:countCharacters() == 0) then
		text:print("There are no characters in the roster.")
		getkey()
		return
	end

	oldMember = text:scrollingSelect(roster, roster.printMember, 1, 8,
		renameHeader, globals.scrollFooter)

	if (not oldMember) then
		return
	end

	text:cdprint(true, false, "What is %s's new name?", oldMember)
	newMember = text:readString(16, true)

	if (roster:nameExists(newMember)) then
		text:cdprint(true, false,
			"There is already a character with that name in the roster.")
		getkey()
		return
	end

	roster:rename(oldMember, newMember)
end

----------------------------------------
-- camp:saveParty()
----------------------------------------
function camp:saveParty()
	local partyName

	text:cdprint(true, false, "Name to save party under?")
	partyName = text:readString(10, true)

	if (not partyName) then
		return
	end

	if (roster:nameExists(partyName)) then
		text:cdprint(true, true, "That name is already in use.")
		return
	end

	roster:writeParty(partyName, party)
end

