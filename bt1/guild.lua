
----------------------------------------
-- add_member()
--
-- Add a member from the roster to the
-- party
----------------------------------------
local function add_member()
	local member

	if (party:isOccupied(7)) then
		splashMessage("\n\nNo room to add new members.");

		return
	end

	member = text_scrollSelect(roster.toArray(), roster.printMember)
	if (not member) then
		return
	end

	text_clear()

	if (roster.isParty(member)) then
		party:addParty(roster.getParty(member))
	else
		if (party:findByName(member)) then
			splashMessage("\n\nThat member is already in the party.")
			return
		end

		party:addCharacter(roster.getCharacter(member))
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
		roster.saveCharacter(newchar)
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

	member = text_scrollSelect(roster.toArray(), roster.printMember)
	if (not member) then
		return
	end

	if (party:findByName(member)) then
		splashMessage("You can't delete a member in your party from the disk.")
		return
	end

	text_clear()
	text_print("This will permanently remove " .. member .. " from the disk! Do you wish to do this?")

	if (not getYesNo()) then
		return
	end

	roster.removeCharacter(member)
end

----------------------------------------
-- leave_game()
----------------------------------------
local function leave_game()
	text_clear()
	text_print("\nLeave the game?")

	if (getYesNo()) then
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

	if (not party:isOccupied(1)) then
		splashMessage("\nWhat party!")

		return
	end

	text_clear()
	text_print("\nPick the party member to save to disk and remove from the party.")
	printCancel()

	inkey = getkey()
	if ((inkey > "0") and (inkey < "7")) then
		member = party:removeCharacter(tonumber(inkey))
		dprint(member)

		roster.updateCharacter(member)

		--party:pack()
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

	if (not party:isOccupied(1)) then
		splashMessage("\nWhat party!")

		return
	end

	text_clear()
	text_print("\nName to save party under?\n")

	name = text_readString()
	if (name:len() == 0) then
		return
	end

	if (roster.nameExists(name)) then
		text_clear()
		text_print("That name is already in use.")

		if (not roster.isParty(name)) then
			printContinue()
			getkey()

			return
		end

		text_print("\nDo you still want to use it?")
		if (not getYesNo()) then
			return
		end
	end

	roster.saveParty(name)
end

local function enter()
	local inkey;

	dprint("guild.enter() called")
	bigpic.drawBigpic("PIC_GUILDINT");
	bigpic.setTitle("The Guild");

	while 1 do
		text_clear()
		text_print("Thou art in the Guild of Adventurers.\n");
		text_print("\nAdd member");
		text_print("\nRemove member");
		text_print("\nCreate a member");
		text_print("\nDelete a member");
		text_print("\nSave party");
		text_print("\nLeave game");
		text_print("\nEnter the city");

		inkey = getkey();

		if (inkey == "A") then
			add_member()
		elseif (inkey == "C") then
			create_member()
			bigpic.drawBigpic("PIC_GUILDINT")
			bigpic.setTitle("The Guild")
		elseif (inkey == "D") then
			delete_member()
		elseif (inkey == "E") then
			--if (not party:isOccupied(1)) then
			if (false) then
				text_clear()
				text_print("You must have a party to enter the city.")
				printContinue()
				getkey()
			else
				globals.inDungeon = false
				globals.isNight = false
				globals.gameHour = 7

				currentLevel = city.new("skara", 17, 16, "east")
				--currentLevel = city.new("skara", 25, 15, "west")
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
		elseif ((inkey > "0") and (inkey < "7")) then
			if (party:isOccupied(inkey)) then
				party[tonumber(inkey)]:printCharacter()
				bigpic.drawBigpic("PIC_GUILDINT")
				bigpic.setTitle("The Guild")
			end
		end
	end
end

guild = {
	enter = enter
};
