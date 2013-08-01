require "bigpic"
require "party"
require "roster"
require "character"

----------------------------------------
-- add_member()
--
-- Add a member from the roster to the
-- party
----------------------------------------
local function add_member()
	local member

	if (not party:hasRoom()) then
		text:splashMessage("\n\nNo room to add new members.");

		return
	end

	member = text:scrollingSelect(roster, roster.printMember)
	if (not member) then
		return
	end

	text:clear()

	if (roster:isParty(member)) then
		party:addParty(roster:readParty(member))
	else
		if (party:findByName(member)) then
			text:splashMessage("\n\nThat member is already in the party.")
			return
		end

		party:addCharacter(roster:readCharacter(member))
		party:display()
	end
end

----------------------------------------
-- create_member()
--
-- Create a new character and add it to
-- the roster
----------------------------------------
local function create_member()
	local newchar 
	local attrs

	newchar = character.createCharacter()
	if (newchar) then
		----------------------------------------
		-- Add the character to the roster
		----------------------------------------
		roster:writeCharacter(newchar)
	end
end

----------------------------------------
-- delete_member()
--
-- Delete a member from the on-disk 
-- roster
----------------------------------------
local function delete_member()
	local member

	member = text:scrollingSelect(roster, roster.printMember)
	if (not member) then
		return
	end

	if (party:findByName(member)) then
		text:splashMessage("You can't delete a member in your party from the disk.")
		return
	end

	text:clear()
	text:print("This will permanently remove " .. member .. " from the disk! Do you wish to do this?")

	if (not text:getYesNo()) then
		return
	end

	roster:remove(member)
end

----------------------------------------
-- leave_game()
----------------------------------------
local function leave_game()
	text:clear()
	text:print("\nLeave the game?")

	if (text:getYesNo()) then
		os.exit()
	end
end


----------------------------------------
-- remove_member()
--
-- Remove a member from the party and
-- update the on-disk roster file
----------------------------------------
local function remove_member()
	local inkey
	local member

	if (not party:getFirstCharacter()) then
		text:splashMessage("\nWhat party!")

		return
	end

	text:clear()
	text:print("\nPick the party member to save to disk and remove from the party.")
	text:printCancel()

	inkey = getkey()
	if ((inkey > "0") and (inkey < "7")) then
		member = party:removeCharacter(tonumber(inkey))

		roster:writeCharacter(member)

		party:display()
	end
end

----------------------------------------
-- save_party()
--
-- Save the current party as a party
----------------------------------------
local function save_party()
	local name

	if (not party:getFirstCharacter()) then
		text:splashMessage("\nWhat party!")

		return
	end

	text:clear()
	text:print("\nName to save party under?\n")

	name = text:readString()
	if (name:len() == 0) then
		return
	end

	if (roster:nameExists(name)) then
		text:clear()
		text:print("That name is already in use.")

		if (not roster:isParty(name)) then
			text:printContinue()
			getkey()

			return
		end

		text:print("\nDo you still want to use it?")
		if (not text:getYesNo()) then
			return
		end
	end

	roster:writeParty(name)
end

local function enter()
	local inkey;

	dprint("guild.enter() called")
	bigpic:drawImage("PIC_GUILDINT")
	bigpic:setTitle("The Guild")

	while 1 do
		text:clear()
		text:print("Thou art in the Guild of Adventurers.\n");
		text:print("\nAdd member");
		text:print("\nRemove member");
		text:print("\nCreate a member");
		text:print("\nDelete a member");
		text:print("\nSave party");
		text:print("\nLeave game");
		text:print("\nEnter the city");

		inkey = getkey();

		if (inkey == "A") then
			add_member()
		elseif (inkey == "C") then
			create_member()
			bigpic:drawImage("PIC_GUILDINT")
			bigpic:setTitle("The Guild")
		elseif (inkey == "D") then
			delete_member()
		elseif (inkey == "E") then
			if (not party:getFirstCharacter()) then
				text:clear()
				text:print("You must have a party to enter the city.")
				text:printContinue()
				getkey()
			else
				globals.inDungeon = false
				globals.isNight = false
				globals.gameHour = 7

				currentLevel = city:new("skara")
				--currentLevel:enter(2, 3, "south")
				currentLevel:enter(17, 16, "east")
				--currentLevel:enter(25, 15, "east")
				globals.gameState = globals.STATE_CITY
				return
			end
		elseif (inkey == "L") then
			leave_game()
		elseif (inkey == "N") then
			party:reorder()
		elseif (inkey == "Q") then
			os.exit()
		elseif (inkey == "R") then
			remove_member()
		elseif (inkey == "S") then
			save_party()
		elseif (inkey == "T") then
			dprint(party:randomMeleeCharacter().name)
		elseif ((inkey > "0") and (inkey < "7")) then
			local c = party:isOccupied(inkey)
			if (c) then
				c:printCharacter()
				bigpic:drawImage("PIC_GUILDINT")
				bigpic:setTitle("The Guild")
			end
		end
	end
end

guild = {
	enter = enter
};
