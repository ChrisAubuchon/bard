guild = {}

----------------------------------------
-- addMember()
----------------------------------------
function guild:addMember()
	local member

	if (not party:hasRoom()) then
		text:csplash(true, true, "\n\nNo room to add new members.")
		return	
	end

	member = text:scrollingSelect(roster, roster.printMember, 0, 10, nil, globals.selectFooter)
	if (not member) then
		return
	end

	log:print(log.LOG_DEBUG, member)

	text:clear()

	if (roster:isParty(member)) then
		party:addParty(roster:readParty(member))
	else
		if (party:findByName(member)) then
			text:csplash(true, true, 
				"\n\nThat member is already in the party."
				)
			return
		end
		
		party:addCharacter(roster:readCharacter(member))
		party:display()
	end
end

----------------------------------------
-- convertBardI()
----------------------------------------
function guild:convertBardI()
	local member
	local biRoster

	biRoster = roster:new("bt1")

	local function doConvert(inName)
		local m

		if (party:findByName(inName)) then
			return false
		end

		m = roster:convert(biRoster.characters[inName], 1)
		party:addCharacter(m)

		return true
	end

	repeat
		member = text:scrollingSelect(biRoster, biRoster.printMember)
		if (biRoster:isParty(member)) then
			local c

			for _,c in pairs(biRoster:readParty(member)) do
				if (biRoster:nameExists(c)) then
					doConvert(c)
				end
			end
		elseif (member) then
			if (not doConvert(member)) then
				text:csplash(true, true,
				"\n\nThat member is already in the party."
				)
				return
			end
		end
	until (not member)
end

----------------------------------------
-- deleteMember()
----------------------------------------
function guild:deleteMember()
	local member

	member = text:scrollingSelect(roster, roster.printMember)
	if (not member) then
		return
	end

	if (party:findByName(member)) then
		text:csplash(true, true, 
				"You can't delete a member in your party " ..
				"from the disk."
			)
		return
	end

	text:cdprint(true, false,
		"This will permanently remove %s from the disk! Do you " ..
		"wish to do this?", member)

	if (not text:getYesNo()) then
		return
	end

	roster:remove(member)
end

----------------------------------------
-- renameMember()
----------------------------------------
function guild:renameMember()
	local member
	local newName

	member = text:scrollingSelect(roster, roster.printMember)
	if (not member) then
		return
	end

	if (party:findByName(member)) then
		text:csplash(true, true, "You can't rename a member in " .. 
					"your party.")
		return
	end

	repeat
		text:cdprint(true, false, "What is the new name for %s.\n\n", 
				member)
		newName = text:readString()
		if (newName:len() == 0) then
			return
		end

		if (roster:nameExists(newName)) then
			text:csplash(true, true, "That name is already in use.")
		end

		roster:renameMember(member, newName)
		return
	until (false)
end

----------------------------------------
-- diskOptions()
----------------------------------------
function guild:diskOptions()
	local inkey

	local function doIntro()
		text:clear()
		text:setCursor(0, 2)
		text:print("\nDelete a member"
			.. "\nLoad Bard I member"
			.. "\nRename a member"
			.. "\nExit disk options"
			)
	end

	doIntro()

	repeat
		inkey = getkey()
		if (inkey == "D") then
			self:deleteMember()
			doIntro()
		elseif (inkey == "L") then
			self:convertBardI()
			doIntro()
		elseif (inkey == "R") then
			self:renameMember()
			doIntro()
		end
	until (inkey == "E")
end

----------------------------------------
-- quitGame()
----------------------------------------
function guild:quitGame()
	local c
	text:cdprint(true, false, "\nQuit the game?")
	if (text:getYesNo()) then
		for c in party:iterator() do
			party:removeCharacter(c)
			roster:writeCharacter(c)
		end
		os.exit(0)
	end

	text:clear()
end

----------------------------------------
-- removeMember()
----------------------------------------
function guild:removeMember()
	local inkey
	local member

	if (not party:getFirstCharacter()) then
		text:csplash(true, true, "\nWhat member!")
		return
	end

	text:cdprint(true, false, 
				"\nPick the party member to save to " ..
				"disk and remove from the party."
			)
	text:printCancel()

	inkey = getkey()
	if ((inkey > "0") and (inkey < "8")) then
		member = party:removeCharacter(tonumber(inkey))
		roster:writeCharacter(member)
		party:display()
	end
end

----------------------------------------
-- createMember()
----------------------------------------
function guild:createMember()
	local newchar

	newchar = character.createCharacter()
	if (newchar) then
		roster:writeCharacter(newchar)
	end
end

----------------------------------------
-- doEnter()
----------------------------------------
function guild:doEnter()
	local inkey

	self:resetBigpic()

	repeat
		text:cdprint(true, false, 
			"Thou art in the Guild of Adventurers."..
			"\n\nAdd member"	..
			"\nRemove member"	..
			"\nCreate a member"	..
			"\nSave party"		..
			"\nQuit game"		..
			"\nEnter the city"	..
			"\nDisk options"	..
			"\nLoad saved game"	
			)
		inkey = getkey()

		if ((inkey > "0") and (inkey < "8")) then
			local c = party:isOccupied(inkey)
			if (c) then
				c:printCharacter()
				self:resetBigpic()
			end
		elseif (inkey == "A") then
			self:addMember()
		elseif (inkey == "C") then
			self:createMember()
			self:resetBigpic()
		elseif (inkey == "D") then
			self:diskOptions()
		elseif (inkey == "E") then
			if (not party:getFirst()) then
				text:csplash(true, true,
					"You must have a party to enter " ..
					"the city."
					)
			else
				globals.inDungeon = false
				globals.isNight = false
				globals.gameHour = 7

				globals.gameState = globals.STATE_INGAME
				level:toLevel("city", globals.guildCity)
				party.light:__activate(-1)
				party.light.distance = 5
				party.light.seeSecret = true
				party.light.active = true

				return
			end
		elseif (inkey == "L") then
			globals.gameState = globals.STATE_INGAME
			globals:restoreGame()
			return
		elseif (inkey == "N") then
			party:reorder()
		elseif (inkey == "R") then
			self:removeMember()
		elseif (inkey == "Q") then
			self:quitGame()
		elseif (inkey == "G") then
			collectgarbage()
		end
	until (inkey == "E")
end

----------------------------------------
-- guild:enter()
----------------------------------------
function guild:enter()
	local self = object:new()

	self:addParent(guild)
	self:addParent(building:new("The Guild", "PIC_GUILDINT"))

	local c = diskio:readTable("gameState", true)
	if (not globals.guildCity) then
		if (not c.guildCity) then
			globals.guildCity = "Tangramayne"
		else
			globals.guildCity = c.guildCity
		end
	else
		c.guildCity = globals.guildCity
		diskio:writeTable(c, "gameState")
	end

	self:doEnter()
end
