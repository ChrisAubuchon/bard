dunSquares = {}

----------------------------------------
-- battle()
--
-- Helper for dungeon square battles
----------------------------------------
function dunSquares:battle(...)
	if (not battle:new(...)) then
		if (globals.partyDied) then
			return false
		end
		text:clear()
		self.currentSquare = self.previousSquare
		return false
	else
		self.currentSquare:clearCode()
		return true
	end
end

----------------------------------------
-- longStairs()
----------------------------------------
function dunSquares:longStairs()
	local rval

	text:cdprint(true, false, 
		"There are stairs here, which seem to go a long way up. " ..
		"Do you wish to take them?"
		)
	
	if (text:getYesNo()) then
		text:clear()
		self:exitToCity()
		currentLevel.currentSquare = currentLevel:getSq("1-1")
	else
		text:clear()
	end
end

----------------------------------------
-- spiderGod()
----------------------------------------
function dunSquares:spiderGod()
	local inkey

	bigpic:drawImage("PIC_SPIDER")
	bigpic:setTitle("Spidergod")
	text:cdprint(true, false, "There is a staute of a large spider here. ")
	text:print("You can\n\nSearch it")
	text:printExit()

	repeat
		inkey = getkey()

		if (inkey == "S") then
			text:cdprint(false,true,"\n\nThat was not a wise move!")
			self:battle("M_SPINNER", 1)
			return
		end
	until (inkey == "E")
	self.currentSquare = self.previousSquare
end

----------------------------------------
-- lightBeam()
----------------------------------------
function dunSquares:lightBeam()
	local inkey

	if (globals.isNight) then
		return
	end

	text:cdprint(true, false, 
		"A light beam from the surface is mirrored down here " ..
		"and focused to a burning ray, blocking the corridor. " ..
		"Do you wish to:\n\nTry crossing"
		)
	text:printExit()

	repeat
		inkey = getkey()

		if (inkey == "T") then
			local c

			for c in party:iterator() do
				local action = btAction:new()
				action.outData.damage = 30
				c:doDamage(action)
				if (globals.partyDied) then
					return
				end
			end
			party:display()
			text:cdprint(true, true,
				"\nYou try but are unable to pass through " ..
				"the burning ray of light!"
				)
			self.currentSquare = self.previousSquare
			return
		end
	until (inkey == "E")
	self.currentSquare = self.previousSquare
end

----------------------------------------
-- tarjanMouth()
----------------------------------------
function dunSquares:tarjanMouth()
	bigpic:drawImage("PIC_MAGICMOUTH")
	bigpic:setTitle("Mouth...")
	text:clear()
	text:csplash(true, true, 
		"A magic mouth on the wall speaks: Know this, that a man " ..
		"called Tarjan, thought to many to be insane, had through " ..
		"wizardly powers"
		)
	text:clear()
	text:csplash(true, true, 
		"...proclaimed himself a god in Skara Brae a hundred " ..
		"years ago. His image is locked in stone until made " ..
		"whole again..."
		)
	text:clear()
	self.currentSquare:clearCode()
end

----------------------------------------
-- basharKavilor()
----------------------------------------
function dunSquares:basharKavilor()
	text:clear()
	bigpic:drawImage("PIC_SMAGE")
	bigpic:setTitle("Bashar Kavilor")
	text:csplash(true, true, 
		"You have entered the living chambers of Bashar Kavilor, " ..
		"the High Priest. He screams, \"Infidel! Face now the " ..
		"wrath of a servant of the Mad One!\"\n\n"
		)
	text:cdprint(true, true, "\n\nI think you're in trouble.")
	self:battle("M_MASTER_SORCERER", 1)
	return
end

----------------------------------------
-- sphynx()
----------------------------------------
function dunSquares:sphynx()
	bigpic:drawImage("PIC_TREASURE")
	bigpic:setTitle("")
	text:clear()
	text:csplash(true, true, "This is the High Priest's treasure chamber. " ..
		"His pet dragon, Sphynx, peers at you hungily. \"Sphynx " ..
		"eat now,\" it growls."
		)
	self:battle("M_GREY_DRAGON", 1)
end
	
----------------------------------------
-- witchKing()
----------------------------------------
function dunSquares:witchKing()
	text:clear()
	bigpic:drawImage("PIC_SPECTRE")
	bigpic:setTitle("Witch King")
	text:csplash(true, true, "Before you lies the ancient burial chamber " ..
		"of King Aildrek, now a creature of darkness. \"Now, " ..
		"mortal man, I will drink of your souls,\" he hisses."
		)
	if (self:battle("M_SPECTRE", 1)) then
		local c
		c = party:giveItem("Eye", true)
		if (c) then
			text:cdprint(true, true, "%s has found an eye.",
				c.name)
		end
	end
end

----------------------------------------
-- baronsThrone()
----------------------------------------
function dunSquares:baronsThrone()
	local inkey
	local c

::restart::
	if (self.direction == "south") then
		return
	end

	bigpic:setTitle("Throne")
	text:cdprint(true, false, 
		"You are at the Baron's throne. Who wishes to sit on it?"
		)

	inkey = getkey()
	c = party:readMember(inkey)
	if (not c) then
		self.currentSquare = self.previousSquare
		return
	end

	if (c.class ~= "Bard") then
		local action = btAction:new()
		action.outData.damage = 15
		action.target = c
		c:doDamage(action)
		if (globals.partyDied) then
			return
		end
		text:cdprint(true, true, "\n\nOuch! That hurt!")
		party:display()
		goto restart
	else
		text:cdprint(true, true, 
			"\n\nThe sound of grating stone is heard."
			)
		self.currentSquare.north.gfx = false
		self.currentSquare.north.isWall = false
		self.currentSquare:clearCode()
	end
end

----------------------------------------
-- guardCaptain()
----------------------------------------
function dunSquares:guardCaptain()
	text:clear()
	bigpic:drawImage("PIC_THIEF")
	bigpic:setTitle("Guard Captain")
	text:csplash(true, true, 
		"You see the Captain of the Guard standing before you.\n" ..
		"\"Die, scum!\" he screams as he runs insanely toward you."
		)
	self:battle("M_MASTER_NINJA", 1)
end

----------------------------------------
-- robedMen()
----------------------------------------
function dunSquares:robedMen()
	local foundRobes = false
	local c

	text:clear()
	bigpic:drawImage("PIC_NOMAD")
	bigpic:setTitle("Robed Men")
	text:csplash(true, true, 
		"Before you stand six warriors, dressed in bright green " ..
		"robes. \"Intruders!\" They scream battle cries as they " ..
		"run toward you with drawn swords."
		)
	if (self:battle("M_BERSERKER", 6)) then
		for c in party:characterIterator() do
			if (c:giveItem("Robes")) then	
				foundRobes = true
			end
		end

		if (foundRobes) then
			text:cdprint(true, true, "You found some robes.")
			text:clear()
		end
		self.currentSquare:clearCode()
	end
end

----------------------------------------
-- crystalSword()
----------------------------------------
function dunSquares:crystalSword()
	text:clear()
	if (not party:hasItem("Crystal Sword")) then
		local c

		c = party:giveItem("Crystal Sword")
		if (c) then
			text:cdprint(true,true, 
				"%s has found a Crystal sword.", c.name
				)
			text:clear()
		end
	end
	self.currentSquare:clearCode()
end

----------------------------------------
-- vampireOldMan
----------------------------------------
function dunSquares:vampireOldMan()
	local inkey

	bigpic:drawImage("PIC_FRED")
	bigpic:setTitle("Old Man")

	local function printIntro()
		text:csplash(true, true, 
			"A wizened man appears before you, saying, "..
			"\"Answer this riddle to pass:"
			)
		text:csplash(true, true, 
			"\"Once man alive\nNow living death\nIt drinketh " ..
			"blood\nN' stealeth breath.\""
			)
		text:cdprint(true,false,"Will you:\n\nAnswer riddle\nFight him")
		text:printExit()
	end

	local function checkAnswer()
		local answer

		text:cdprint(true, false, "What say thee?")
		answer = text:readString()
		if (answer ~= "VAMPIRE") then
			text:cdprint(true, true, "\"Wrong,\" he says.")
			text:clear()
			return false
		end

		text:cdprint(true, true, 
			"\"Pass at your leisure,\" the Master Sorcerer says."
			)
		text:clear()
		return true
	end

	printIntro()

	repeat
		inkey = getkey()
		if (inkey == "A") then
			if (checkAnswer()) then
				self.currentSquare:clearCode()
				return
			end
			printIntro()
		elseif (inkey == "F") then
			self:battle("M_MASTER_SORCERER", 1)
			return
		end
	until (inkey == "E")

	text:clear()
	self.currentSquare = self.previousSquare
end

----------------------------------------
-- silverSquare()
----------------------------------------
function dunSquares:silverSquare()
	text:clear()
	if (not party:hasItem("Silvr Square")) then
		local c

		c = party:giveItem("Silvr Square")
		if (c) then
			text:cdprint(true, true, 
				"%s has found a silver square.", c.name
				)
			text:clear()
		end
		self.currentSquare:clearCode()
	end
end

----------------------------------------
-- shieldsMouth
----------------------------------------
function dunSquares:shieldsMouth()
	local answer

	bigpic:drawImage("PIC_MAGICMOUTH")
	bigpic:setTitle("Mouth...")
	text:csplash(true, true, 
		"A magic mouth on the wall speaks to you, saying this riddle:"
		)
	text:cdprint(true, false, 
		"Past warscapes fought by men long dead, and treasures " ..
		"lost on bloodied fields, the One god lifts his " ..
		"thorn-crowned head, and lays a strength on friendly..\n\n"
		)

	answer = text:readString()
	if (answer ~= "SHIELDS") then
		text:cdprint(true, true, "\n\"Wrong!\"")
		text:clear()
	else
		local c

		c = party:giveItem("Ybarrashield")
		if (c) then
			text:cdprint(true, true, "%s found a shield.", c.name)
		end
	end
	self.currentSquare:clearCode()
end

----------------------------------------
-- partyHasRobes()
----------------------------------------
function dunSquares:partyHasRobes()
	local c

	text:clear()

	for c in party:characterIterator() do
		if (not c:hasItem("Robes")) then
			bigpic:drawImage("PIC_NOMAD")
			bigpic:setTitle("Robed Men")
			text:csplash(true, true, 
				"The legions of Baron Harkyn stand before " ..
				"you, recognizing you as intruders, " ..
				"\"Death to them!\" they scream."
				)
			self:battle("M_BERSERKER", 99,
				    "M_BERSERKER", 99,
				    "M_BERSERKER", 99,
				    "M_BERSERKER", 99
				   )
			return
		end
	end
end

----------------------------------------
-- eyeStatue()
----------------------------------------
function dunSquares:eyeStatue()
	local inkey

	bigpic:setBigpic("PIC_MADGOD", "Old statue")

	local function printIntro()
		text:cdprint(true, false, 
			"An old statue of a fifteen foot tall warrior " ..
			"stands before you. Will you:\n\nApproach it"
			)
		text:printExit()
	end

	printIntro()
	repeat
		inkey = getkey()
		if (inkey == "A") then
			local c

			c = party:hasItem("Eye")
			dprint(c)
			if (c) then
				text:cdprint(true, true, 
					"An Eye levitates from %s and goes " ..
					"into the statue's empty socket.", 
					c.name
					)
				c.inventory:dropItem("Eye")
				if (self:battle("M_MAD_GOD", 1)) then
					text:cdprint(true, true, 
						"You are being teleported!"
						)
					self:exitToCity()
					currentLevel.currentSquare = currentLevel:getSq("28-2")
					return
				end
				return
			else
				text:cdprint(true, true, "It appears harmless.")
				printIntro()
			end
		end
	until (inkey == "E")

	text:clear()
	self.currentSquare = self.previousSquare
end

----------------------------------------
-- skullTavernQuestion()
----------------------------------------
function dunSquares:skullTavernQuestion()
	local inkey

	text:clear()
	bigpic:setBigpic("PIC_FRED", "An old man")

	local function printIntro()
		text:csplash(true, true, 
			"An old man stands before you saying, \"Answer " ..
			"this question to pass:\n\nWhat tavern lies off " ..
			"Night Archer street?\""
			)
	end

	local function checkAnswer()
		local answer

		text:cdprint(true, false, "\"Speak up, youngster!\"\n")
		answer = text:readString()
		if ((answer == "SKULL") or (answer == "SKULL TAVERN")) then	
			return true
		end

		return false
	end

	printIntro()

	repeat
		text:cdprint(true, false, 
			"Will you:\n\nAnswer question\nFight him"
			)
		text:printExit()

		inkey = getkey()

		if (inkey == "A") then
			if (checkAnswer()) then
				self.currentSquare:clearCode()
				text:cdprint(true, true, 
					"\"Pass, oh learned ones,\" he says."
					)
				text:clear()
				return
			else
				text:cdprint(true, true, 
					"\"Wrong,\" the old man says."
					)
				printIntro()
			end
		elseif (inkey == "F") then
			self:battle("M_OLD_MAN", 1, 
					"M_LESSER_DEMON", 2,
					"M_LESSER_DEMON", 2
				)
			return
		end
	until (inkey == "E")

	self.currentSquare = self.previousSquare
end

----------------------------------------
-- stoneGolemMouth
----------------------------------------
function dunSquares:stoneGolemMouth()
	local answer

	bigpic:setBigpic("PIC_MAGICMOUTH", "Mouth...")
	text:cdprint(true, false, 
		"A magic mouth on the wall speaks to you, saying this:\n\"" ..
		"Name the one of cold, foretold twofold:\"\n\n"
		)

	answer = text:readString()
	if (answer == "STONE GOLEM") then
		self.currentSquare = self:getSqXY(9, 8)
	else
		text:cdprint(true, true, "\n\"Wrong!\"")
		text:clear()
		self.currentSquare = self.previousSquare
	end
end

----------------------------------------
-- sinisterMouth()
----------------------------------------
function dunSquares:sinisterMouth()
	local answer

	bigpic:setBigpic("PIC_MAGICMOUTH", "Mouth...")
	text:cdprint(true, false, 
		"A magic mouth on the wall speaks to you, saying \"" ..
		"Name the endless byway and your path shall be guaranteed:\"\n"
		)

	answer = text:readString()
	if (answer == "SINISTER") then
		local sq
		text:cdprint(false, true, "\n\"Right...\"")

		sq = self:getSqXY(6,1)
		sq.south.isWall = false
		sq.south.isDoor = true
		sq.south.gfx = "door"
		self.currentSquare:clearCode()
	else
		text:cdprint(false, true, "\n\"Wrong!\"")
		self.currentSquare = self.previousSquare
	end
end

----------------------------------------
-- silverTriangle()
----------------------------------------
function dunSquares:silverTriangle()
	text:clear()

	if (not party:hasItem("Silvr Triang")) then
		local c

		c = party:giveItem("Silvr Triang")
		if (c) then
			text:cdprint(true, true, 
				"%s has found a silver triangle.", c.name
				)
			text:clear()
		end
		self.currentSquare:clearCode()
	end
end

----------------------------------------
-- crystalStatue()
----------------------------------------
function dunSquares:crystalStatue()
	local inkey

	local function printIntro()
		bigpic:setBigpic("PIC_STATUE", "Golem!")
		text:cdprint(true, false, 
			"A crystal statue comes to life before you, ready " ..
			"to attack! Will you:\n\nFight it"
			)
		text:printExit()
	end

	printIntro()

	repeat
		inkey = getkey()

		if (inkey == "F") then
			if (battle:new("M_CRYSTAL_GOLEM", 1)) then
				if (party:isItemEquipped("Crystal Sword")) then
					self.currentSquare:clearCode()
					return
				end
				text:cdprint(false, true, 
					"Something is happening to the " ..
					"golem's shattered body..."
					)
				printIntro()
			else
				if (not globals.partyDied) then
					self.currentSquare = self.previousSquare
				end
				return
			end
		end
	until (inkey == "E")

	self.currentSquare = self.previousSquare
end

----------------------------------------
-- onyxKey()
----------------------------------------
function dunSquares:onyxKey()
	local c

	text:clear()
	if (party:hasItem("Onyx Key")) then
		return
	end

	bigpic:setBigpic("PIC_KYLEARAN", "Kylearan")
	text:csplash(false, true,
		"Greetings! I am Kylearan, oh perseverent ones, the " ..
		"good wizard of Skara Brae. Put away your weapons, for I " ..
		"am your ally, not an enemy\n"
		)
	text:csplash(true, true, 
		"You have passed my tests, and can now continue on to " ..
		"your true quest, the source of evil in skara Brae -- " ..
		"Mangar the Dark, my nemesis\n"
		)
	text:csplash(true, true, 
		"Mangar's tower, at the opposite corner of Skara Brae, " ..
		"is a five level deathtrap, which can only be entered " ..
		"by use of an onyx key.\n"
		)
	text:cdprint(true, true, 
		"Good luck, and remember, seeing is not always believing.\n\n"
		)

	c = party:giveItem("Onyx Key")
	if (c) then
		text:cdprint(true, true, "%s got a key.\n\n", c.name)
	end
	text:cdprint(false, true, "Kylearan vanishes.")
	self.currentSquare:clearCode()
end

----------------------------------------
-- mouthFour()
--
-- This magic mouth moves you up a 
-- level.
----------------------------------------
function dunSquares:mouthFour()
	bigpic:setBigpic("PIC_MAGICMOUTH", "Mouth...")

	text:csplash(true, true, 
		"A magic mouth on the wall speaks to you, saying this:\n\"" ..
		"Perseverence in all things.\""
		)
	text:clear()
	self:changeLevel(1)
end

----------------------------------------
-- silverCircle()
----------------------------------------
function dunSquares:silverCircle()
	local answer
	local c

	if (party:hasItem("Silvr Circle")) then
		return	
	end

	bigpic:setBigpic("PIC_MAGICMOUTH", "Mouth...")
	text:cdprint(true, false, 
		"A magic mouth on the wall speaks to you, saying this:\n\"" ..
		"Two shapes yours,\n one's around;\n Speak the shape\n and " ..
		"final found\"\n\n"
		)

	answer = text:readString()
	if (answer ~= "CIRCLE") then
		text:cdprint(true, true, "\n\"Wrong!\"")
		text:clear()
		self.currentSquare = self.previousSquare
		return
	end

	c = party:giveItem("Silvr Circle")
	if (c) then
		text:cdprint(true,true, "%s has found a silver circle.", c.name)
	end
	self.currentSquare:clearCode()
end

----------------------------------------
-- masterKey()
----------------------------------------
function dunSquares:masterKey()
	local inkey

	if (party:hasItem("Master Key")) then
		return
	end

	bigpic:setBigpic("PIC_FRED", "Keymaster")
	text:csplash(true, true, 
		"An old man is here, and says to you:\n\"I am the " ..
		"Keymaster. I have something to sell you for the low " ..
		"price of 50000 gold pieces.\""
		)
	self.currentSquare:clearCode()

	----------------------------------------
	-- printIntro() helper function
	----------------------------------------
	local function printIntro()
		text:cdprint(true, false, 
				"\"What say thee?\"\n\nBuy item\nFight him")
		text:printExit()
	end

	----------------------------------------
	-- getPayer() helper function
	----------------------------------------
	local function getPayer(inPayer)
		if (inPayer.inventory:isFull()) then
			text:cdprint(false, true, "Your pockets are full.")
			return false
		end

		if (inPayer.gold < 50000) then
			text:cdprint(false, true, "Not enough gold.")
			return false
		end

		return true
	end

	printIntro()
	repeat
		inkey = getkey()

		if (inkey == "B") then
			local c

			text:cdprint(true, false, "Who will pay?")
			c = party:readSlot()
			if (c) then
				text:clear()
				if (getPayer(c)) then
					c:giveItem("Master Key")
					c.gold = c.gold - 50000
					text:cdprint(false, true, 
						"%s got a key.", c.name)
					return
				end
			end
			printIntro()
		elseif (inkey == "F") then
			text:csplash(true, true, 
				"The man speaks briefly and 6 friends " ..
				"appear. \"Most unfriendly,\" he says. " ..
				"His friends look very angry."
				)
			self:battle("M_WRAITH", 3, 
				    "M_WRAITH", 3, 
				    "M_OLD_MAN", 1)
			return
		end
	until (inkey == "E")
end

----------------------------------------
-- sevenAnswers()
----------------------------------------
function dunSquares:sevenAnswers()
	local i
	local answer
	local sq
	local correct = {
		"LIE", "WITH", "PASSION", "AND", "BE", "FOREVER", "DAMNED"
	}

	bigpic:setBigpic("PIC_MAGICMOUTH", "Mouth...")
	text:cdprint(true, false, 
		"A magic mouth on the wall speaks to you, saying this:\n\"" ..
		"Speak the seven of the One God, in Sequence, and seek " ..
		"the lost stairs:\""
		)
	self.currentSquare:clearCode()

	for i = 1,7 do
		text:print("\n")
		timer:delay(1)
		answer = text:readString()
		if (answer ~= correct[i]) then
			text:cdprint(true, true, "\n\"Wrong!\"")
			text:clear()
			return
		end
	end

	text:cdprint(true, true, "\"Well done, oh faithful one.\"")
	text:clear()
	sq = self:getSqXY(3,9)
	sq.isStairs = "if (currentLevel:doStairs(\"Up\")) then currentLevel:changeLevel(1) end"
end

----------------------------------------
-- largeCoffin
----------------------------------------
function dunSquares:largeCoffin()
	local inkey

	text:cdprint(true, false, 
		"There is a large black coffin in this tomb. " ..
		"Do you wish to:\n\nOpen it"
		)
	text:printExit()

	repeat
		inkey = getkey()

		if (inkey == "O") then
			text:csplash(true, true, 
				"A Vampire Lord rises from the coffin. " ..
				"He seems rather disturbed."
				)
			self:battle("M_VAMPIRE_LORD", 1)
			return
		end
	until (inkey == "E")

	self.currentSquare = self.previousSquare
end

----------------------------------------
-- sleepingDragons()
----------------------------------------
function dunSquares:sleepingDragons()
	if (party.song.active) then
		text:csplash(true, true, 
			"Your music caused two sleeping dragons to " ..
			"continue slumbering."
			)
		self.currentSquare:clearCode()
		return
	end

	text:csplash(true, true, 
		"Two sleeping dragons awaken at your passage. \"Hmm, " ..
		"humans for dinner again, Gor,\" says the small one. " ..
		"\"Why don't we get any pork up here?\""
		)
	self:battle("M_RED_DRAGON", 1, "M_RED_DRAGON", 1)
end

----------------------------------------
-- thorFigurine()
----------------------------------------
function dunSquares:thorFigurine()
	local answer
	local c

	bigpic:setBigpic("PIC_MAGICMOUTH", "Mouth...")
	text:cdprint(true, false, 
		"A magic mouth on the wall speaks to you, saying this:\n\"" ..
		"Name the greatest son of Odin and win his aid.\""
		)

	answer = text:readString()
	if (answer ~= "THOR") then
		text:cdprint(true, true, "\n\"Wrong!\"")
	else
		c = party:giveItem("Thor Fgn")
		if (c) then
			text:cdprint(true, true, 
				"%s has found a figurine.", c.name)
			text:clear()
		end
	end
	self.currentSquare:clearCode()
end

----------------------------------------
-- swapDoors()
----------------------------------------
function dunSquares:swapDoors()
	globals.swapWallsAndDoors = true
	self.currentSquare:clearCode()
end

----------------------------------------
-- silverDest
----------------------------------------
function dunSquares:silverDest()
	text:cdprint(true, true, 
		"On the wall to the north are etched 3 geometric shapes: " ..
		"a square, a circle, and a triangle."
		)
	self.currentSquare:clearCode()

	if (party:hasItem("Silvr Square") and party:hasItem("Silvr Circle") and
	    party:hasItem("Silvr Triang")) then
		self.currentSquare.north.isWall = false
		self.currentSquare.north.isDoor = true
		self.currentSquare.north.gfx = "door"
	end

	return
end

----------------------------------------
-- spectreSnare()
----------------------------------------
function dunSquares:spectreSnare()
	local answer
	local c

	self.currentSquare:clearCode()

	if (party:hasItem("Spectre Snare")) then
		return
	end

	bigpic:setBigpic("PIC_MAGICMOUTH", "Mouth...")

	text:cdprint(true, false, 
		"A magic mouth on the wall speaks to you, saying this:\n\"" ..
		"What can bind the mightiest foes?\"\n\n"
		)

	answer = text:readString()
	if (answer ~= "SPECTRE SNARE") then
		text:cdprint(true, true, "\n\"Wrong!\"")
		return
	end

	c = party:giveItem("Spectre Snare", true)
	if (c) then
		text:cdprint(true, true, "%s found something.", c.name)
	end

	-- For some reason, the DOS version jumps to a cdprint of '"Wrong!"'
	-- regardless of whether or not a member got the snare.
	--
end

----------------------------------------
-- boilingLiquid()
----------------------------------------
function dunSquares:boilingLiquid()
	local inkey
	local c

	text:cdprint(true, false, 
		"There is a pool of boiling liquid here. Will you:\n\nDive in"
		)
	text:printExit()

	repeat
		inkey = getkey()

		if (inkey == "D") then
			for c in party:iterator() do
				local action = btAction:new()
				action.outData.damage = 5
				action.target = c
				c:doDamage(action)
				if (globals.partyDied) then
					return
				end
			end
			party:display()
			self.currentSquare = self:getSqXY(10, 6)
			return
		end
	until (inkey == "E")

	self.currentSquare = self.previousSquare
end

----------------------------------------
-- mangarTreasure()
----------------------------------------
function dunSquares:mangarTreasure()
	text:csplash(true, true, 
		"This is Mangar's treasure trove. Two dragons are here, " ..
		"and they're very hungry. I think you're in some real trouble."
		)

	self:battle("M_BLACK_DRAGON", 1, "M_BLACK_DRAGON", 1)
end

----------------------------------------
-- mouthOne
----------------------------------------
function dunSquares:mouthOne()
	local c

	bigpic:setBigpic("PIC_MAGICMOUTH", "Mouth...")
	text:cdprint(true, true, 
		"A magic mouth on the wall speaks to you, saying this:\n\"" ..
		"Death to those who would attack the mighty one!\""
		)

	for c in party:iterator() do
		local action = btAction:new()
		action.outData.damage = 15
		c:doDamage(action)
		if (globals.partyDied) then
			return
		end
	end
	self.currentSquare:clearCode()
	party:display()
end

----------------------------------------
-- possessCharacter()
----------------------------------------
function dunSquares:possessCharacter()
	local target
	local action

	repeat
		target = party:randomMeleeCharacter()
	until (not target:isSummon())

	action = btAction:new()
	action.source = target
	action.outData.specialAttack = "possess"
	action.outData.damage = 0
	target:doDamage(action)
	if (globals.partyDied) then
		return
	end

	party:display()
	battle:partyAttack()
	if (globals.partyDied) then
		return
	end
	self.currentSquare:clearCode()
end

----------------------------------------
-- mangar
----------------------------------------
function dunSquares:mangar()
	text:clear()
	bigpic:setBigpic("PIC_MANGAR", "Mangar!")

	text:csplash(true, true, 
		"You stand in the antechamber of Mangar the Dark, evil " ..
		"Archmage of Skara Brae. Mangar glares at you with deep " ..
		"hatred. \"Die, mortals!\" he screams."
		)
	text:csplash(true, true, 
		"With a wave of his hand a few of his close friends drop " ..
		"by. They don't look like a fun group."
		)
	text:clear()
	if (not battle:new("M_DEMON_LORD", 2, "M_VAMPIRE_LORD", 3, 
			   "M_MANGAR", 1)) then
		if (globals.partyDied) then
			return
		end
		self.currentSquare = self.previousSquare
		return
	end

	text:clear()
	bigpic:setBigpic("PIC_KYLEARAN", "Kylearan")

	-- There seems to be a loop waiting for a key here. Not sure why.

	text:csplash(true, true, 
		"Kylearan, the good Archmage, appears before you in a " ..
		"flash of light. \"Well done!\" he cries, beaming. " ..
		"\"Your quest in Skara Brae is now finished!\""
		)
	text:csplash(true, true, 
		"\"The evil one is now defeated, and his spell of winter " ..
		"will soon end.\""
		)
	text:csplash(true, true, 
		"\"You will all, as a symbol of my gratitude, receive " ..
		"a gold and experience bonus of 300,000 units. Fare well " ..
		"and live long!\""
		)
	text:cdprint(true, true, "Kylearan vanishes.")

	for c in party:characterIterator() do
		c.xp = c.xp + 300000
		c.gold = c.gold + 300000
	end

	self.exit = true
	globals.gameState = globals.STATE_GUILD
	currentLevel = false
end


























