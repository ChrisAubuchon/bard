tavern = {}

function tavern:selectOption()
	local inkey

	repeat 
		text:cdprint(true, false, 
			"Hail, travelers! Step to the bar and I'll draw " ..
			"you a tankard."
			)
		text:print("\n\nYou can:\nOrder a drink\nTalk to barkeep")
		text:printExit()

		inkey = getkey()

		if ((inkey > "0") and (inkey < "8")) then
			local c = party:readMember(inkey)
			if (c) then
				c:printCharacter()
				self:resetBigpic()
			end
		elseif (inkey == "O") then
			if (not self:orderDrink()) then
				return
			end
		elseif (inkey == "T") then
			self:talkToBarkeep()
		end
	until (inkey == "E")

	text:clear()
	currentLevel:turnParty("back")
end

local talk = {
	[1]	= "\"Seek knowledge from the sage,\" the barkeep smirks.",
	[2]	= "\"Bribe the dealer to keep him from cheating,\" the " ..
			"barkeep chuckles.",
	[3]	= "\"The Sage charges 500 in gold for information on " ..
			"magical items,\" the barkeep whispers.",
	[4]	= "\"Seek the Masterkey to pass the gates,\" the barkeep " ..
			"stutters.",
	[5]	= "\"The crypt lies in the wilderness,\" the barkeep whispers.",
	[6]	= "\"The Dreamspell can be learned from the rock,\" " ..	
			"the barkeep grumbles.",
	[7]	= "\"Every city has one dungeon within it,\" the barkeep " ..
			"grumbles.",
	[8]	= "\"Traecham solved his task by chilling; he said 'F.....',"..
			"\" the barkeep mutters"
}

function tavern:talkToBarkeep()
	local char
	local tipAmount
	local index

	text:cdprint(true, false, "Who will talk to the barkeep?")
	char = party:readSlot()
	if (not char) then
		return
	end
	if (char:isDisabled()) then
		return
	end

	text:cdprint(true, false, "\"Talk ain't cheap,\" the barkeep says.\n")
	text:print("\nHow much will you tip him?\n")
	tipAmount = text:readNumber()
	if (tipAmount == false) then
		return
	end

	if (tipAmount > char.gold) then
		text:cdprint(false, true, "\nNot enough gold.")
		return
	end

	if (tipAmount == 0) then
		text:cdprint(false, true, "\n\"Money talks, buddy,\" he says.")
		return
	end

	index = math.modf(tipAmount / 100)

	-- It looks like the "dealer cheating" talk option isn't
	-- available in the DOS version. This would make sense since
	-- the casino's are closed.
	--
	local xxx_check_talk_in_game = true
	if (index < 2) then
		index = 1
	elseif (index >= 8) then
		index = 4
	else
		index = index + 1
	end

	text:csplash(true, true, talk[index])
end

----------------------------------------
-- orderDrink()
----------------------------------------
function tavern:orderDrink()
	local char
	local inkey
	local options = {
		A = 3,
		B = 2,
		M = 4,
		F = 6,
		G = 1
	}

	text:cdprint(true, false, "Who will drink?")
	char = party:readSlot()
	if (not char) then
		return true
	end
	if (char:isDisabled()) then
		return true
	end

	text:cdprint(true, false, "Seat thyself, " .. char.name)
	text:print(". We've got...\n")
	text:print("Ale\n")
	text:print("Beer\n")
	text:print("Mead\n")
	text:print("Foul spirits\n")
	text:print("Ginger Ale\n")
	text:printCancel()
	inkey = getkey()
	if (options[inkey] == nil) then
		return true
	end

	text:clear()
	if (options[inkey] > char.gold) then
		text:cdprint(false, true, "\nNot enough gold.")
		return true
	end

	char.gold = char.gold - options[inkey]

	if (inkey == "F") then
		text:print("You don't feel too well.")
	elseif (inkey == "G") then
		text:cdprint(false, true, 
			"The girls in the tavern are not impressed."
			)
		return true
	else
		text:print("Not bad!!")
	end

	if (char.class == "Bard") then
		player.songs = char.cur_level
	end

	timer:delay(5)
	return true
end

function tavern:enter()
	self:resetBigpic()
	self:selectOption()
end

function tavern:new(inTitle)
	local self = {
	}

	btTable.addParent(self, tavern, building:new(inTitle, "PIC_BARINT"))
	btTable.setClassMetatable(self)

	self:enter()
end
