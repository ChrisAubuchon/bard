camp = {}

----------------------------------------
-- camp:enter()
----------------------------------------
function camp:enter()
	local self	= object:new()
	local inkey

	self:addParent(camp)
	self:addParent(building:new("The Ruin", "PIC_CAMP"))


--	XXX - Uncomment when ready
--	party:songTimeout()
--	party:disablePassiveEffects()

	self:resetBigpic()

	repeat
		text:cdprint(true, false, 
			"Thou art in the Camp of Skara Brae.\n\n")

--		if (party:hasEmptySlot()) then
--			text:print("Add a member\n")
--		end

--		if (not party:isEmpty()) then
--			text:print("Remove a member\n")
--		else
--			text:print("Rename a member\n")
--		end

		text:print("Create a member\n" ..
				"Transfer characters\n" ..
				"Delete a member\n")

--		if (not party:isEmpty()) then
--			text:print("Save the party\n")
--			text:print("Enter Wilderness")
--		else

		text:print("Leave the game\n")

		inkey = getkey()

		if (inkey == "A") then
			party:printStatusLine({name="Chris", ac="-16", maxhp="Dead", curhp="800", maxsppt="  0", cursppt="  0", class="Wa"}, 0)
			party:printStatusLine({name="Chris", ac="-16", maxhp="Dead", curhp="800", maxsppt="  0", cursppt="  0", class="Wa"}, 1)
			party:printStatusLine({name="Chris", ac="-16", maxhp="Dead", curhp="800", maxsppt="  0", cursppt="  0", class="Wa"}, 2)
			party:printStatusLine({name="Chris", ac="-16", maxhp="Dead", curhp="800", maxsppt="  0", cursppt="  0", class="Wa"}, 3)
			party:printStatusLine({name="Chris", ac="-16", maxhp="Dead", curhp="800", maxsppt="  0", cursppt="  0", class="Wa"}, 4)
			party:printStatusLine({name="Chris", ac="-16", maxhp="Dead", curhp="800", maxsppt="  0", cursppt="  0", class="Wa"}, 5)
			party:printStatusLine({name="Chris", ac="-16", maxhp="Dead", curhp="800", maxsppt="  0", cursppt="  0", class="Wa"}, 6)
		elseif (inkey == "C") then	
			self:createMember()
		end
	until (inkey == "L")

	globals.gameState = globals.STATE_EXIT
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

			if ((inkey == "F") or (inkey == "M")) then
				return inkey
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

		if (newChar.gender == "M") then
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

