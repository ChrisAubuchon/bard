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

		if ((inkey > "0") and (inkey < "7")) then
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
	[1] =	"\"The guardians can be deadly,\" the barkeep smirks.",
	[2] = 	"\"A taste of wine might turn to ready adventure,\" the " ..
			"barkeep chuckles",
	[3] =	"\"Look for the Review Board on Trumpet Street,\" the " ..
			"barkeep whispers.",
	[4] =	"\"The gates cannot be scaled, but an entrance always " ..
			"exists,\" the barkeep stutters.",
	[5] =	"\"The Stone Golem has been spoken of twofold,\" the " ..
			"barkeep smiles.",
	[6] =	"\"The Spectre Snare can draw in even the mightiest,\" " ..
			"the barkeep grumbles.",
}

function tavern:talkToBarkeep()
	local char
	local tipAmount

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

	if (tipAmount < 200) then
		text:cdprint(true, true, talk[1])
	elseif (tipAmount < 300) then
		text:cdprint(true, true, talk[2])
	elseif (tipAmount < 400) then
		text:cdprint(true, true, talk[3])
	elseif (tipAmount < 500) then
		text:cdprint(true, true, talk[4])
	elseif (tipAmount < 600) then
		text:cdprint(true, true, talk[5])
	elseif (tipAmount < 700) then
		text:cdprint(true, true, talk[6])
	else
		text:cdprint(true, true, talk[4])
	end
end

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
	if (self.title == "Scarlet Bard") then
		text:print("Wine")
		options["W"] = 3
	end
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
	elseif (inkey == "W") then
		text:csplash(true, true, 
			"The barkeep says, \"Go down to the cellar and " ..
			"pick out a bottle.\""
			)
		city:enterDungeon("cellars", 1)
		return false
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
