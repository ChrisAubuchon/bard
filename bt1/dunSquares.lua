dunSquares = {}

function dunSquares:battle(...)
	if (not battle:new(...)) then
		if (globals.partyDied) then
			return false
		end
		text:clear()
		self.currentSquare = self.previousSquare
		return false
	else
		self.currentSquare.clearCode()
		return true
	end
end

function dunSquares:longStairs()
	local rval

	text:cdprint(true, false, "There are stairs here, which seem to go ")
	text:print("a long way up. Do you wish to take them?")
	
	if (text:getYesNo()) then
		text:clear()
		self:exitToCity()
		currentLevel.currentSquare = currentLevel:getSq("1-1")
	else
		text:clear()
	end
end

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

function dunSquares:lightBeam()
	local inkey

	if (globals.isNight) then
		return
	end

	text:cdprint(true, false, "A light beam from the surface is mirrored ")
	text:print("down here and focused to a burning ray, blocking the ")
	text:print("corridor. Do you wish to:\n\nTry crossing")
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
			text:cdprint(true, true,"\nYou try but are unable to pass through the burning ray of light!")
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
	text:splashMessage("A magic mouth on the wall speaks: Know this, that a man called Tarjan, thought to many to be insane, had through wizardly powers")
	text:clear()
	text:splashMessage("...proclaimed himself a god in Skara Brae a hundred years ago. His image is locked in stone until made whole again...")
	text:clear()
	self.currentSquare.clearCode()
end

----------------------------------------
-- basharKavilor()
----------------------------------------
function dunSquares:basharKavilor()
	text:clear()
	bigpic:drawImage("PIC_SMAGE")
	bigpic:setTitle("Bashar Kavilor")
	text:print("You have entered the living chambers of Bashar Kavilor, ")
	text:print("the High Priest. ")
	text:splashMessage("He screams, \"Infidel! Face now the wrath of a servant of the Mad One!\"\n\n")
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
	text:splashMessage("This is the High Priest's treasure chamber. his pet dragon, Sphynx, peers at you hungily. \"Sphynx eat now,\" it growls.")
	self:battle("M_GREY_DRAGON")
end
	
----------------------------------------
-- witchKing()
----------------------------------------
function dunSquares:witchKing()
	text:clear()
	bigpic:drawImage("PIC_SPECTRE")
	bigpic:setTitle("Witch King")
	text:splashMessage("Before you lies the ancient burial chamber of King Aildrek, now a creature of darkness. \"Now, mortal man, I will drink of your souls,\" he hisses.")
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
	text:cdprint(true, false, "You are at the Baron's throne. Who wishes to sit on it?")

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
		text:cdprint(true, true, "\n\nThe sound of grating stone is heard.")
		self.currentSquare.north.gfx = false
		self.currentSquare.north.isWall = false
		self.currentSquare.clearCode()
	end
end

----------------------------------------
-- guardCaptain()
----------------------------------------
function dunSquares:guardCaptain()
	text:clear()
	bigpic:drawImage("PIC_THIEF")
	bigpic:setTitle("Guard Captain")
	text:splashMessage("You see the Captain of the Guard standing before you.\n\"Die, scum!\" he screams as he runs insanely toward you.")
	self:battle("M_MASTER_NINJA", 1)
end

function dunSquares:robedMen()
	local foundRobes = false
	local c

	text:clear()
	bigpic:drawImage("PIC_NOMAD")
	bigpic:setTitle("Robed Men")
	text:print("Before you stand six warriors, dressed in bright green ")
	text:print("robes. \"Intruders!\" ")
	text:splash("They scream battle cries as they run toward you with drawn swords.")
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
		self.currentSquare.clearCode()
	end
end

function dunSquares:crystalSword()
	text:clear()
	if (not party:hasItem("Crystal Sword")) then
		local c

		c = party:giveItem("Crystal Sword")
		if (c) then
			text:cdprint(true,true, "%s has found a Crystal sword.", c.name)
			text:clear()
		end
	end
	self.currentSquare.clearCode()
end

----------------------------------------
-- vampireOldMan
----------------------------------------
function dunSquares:vampireOldMan()
	local inkey

	bigpic:drawImage("PIC_FRED")
	bigpic:setTitle("Old Man")

	local function printIntro()
		text:splashMessage("A wizened man appears before you, saying, \"Answer this riddle to pass:")
		text:splashMessage("\"Once man alive\nNow living death\nIt drinketh blood\nN' stealeth breath.\"")
		text:cdprint("Will you:\n\nAnswer riddle\nFight him")
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

		text:cdprint(true, true, "\"Pass at your leisure,\" the Master Sorcerer says.")
		text:clear()
		return true
	end

	printIntro()

	repeat
		inkey = getkey()
		if (inkey == "A") then
			if (checkAnswer()) then
				self.currentSquare.clearCode()
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
			text:cdprint(true, true, "%s has found a silver square.", c.name)
			text:clear()
		end
		self.currentSquare.clearCode()
	end
end

----------------------------------------
-- shieldsMouth
----------------------------------------
function dunSquares:shieldsMouth()
	local answer

	bigpic:drawImage("PIC_MAGICMOUTH")
	bigpic.setTitle("Mouth...")
	text:splashMessage("A magic mouth on the wall speaks to you, saying this riddle:")
	text:cdprint(true, false, "Past warscapes fought by men long dead, and treasures lost on bloodied fields, ")
	text:print("the One god lifts his thorn-crowned head, and lays a strength on friendly..\n\n")

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
	self.currentSquare.clearCode()
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
			text:splashMessage("The legions of Baron Harkyn stand before you, recognizing you as intruders, \"Death to them!\" they scream.")
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
		text:cdprint(true, false, "An old statue of a fifteen foot tall warrior stands before you. Will you:\n\nApproach it")
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
				text:cdprint(true, true, "An Eye levitates from %s and goes into the statue's empty socket.", c.name)
				c.inventory:dropItem("Eye")
				if (self:battle("M_MAD_GOD", 1)) then
					text:cdprint(true, true, "You are being teleported!")
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
		text:splashMessage("An old man stands before you saying, \"Answer this question to pass:\n\nWhat tavern lies off Night Archer street?\"")
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
		text:cdprint(true, false, "Will you:\n\nAnswer question\nFight him")
		text:printExit()

		inkey = getkey()

		if (inkey == "A") then
			if (checkAnswer()) then
				self.currentSquare.clearCode()
				text:cdprint(true, true, "\"Pass, oh learned ones,\" he says.")
				text:clear()
				return
			else
				text:cdprint(true, true, "\"Wrong,\" the old man says.")
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
	text:cdprint(true, false, "A magic mouth on the wall speaks to you, saying this:\n\"")
	text:print("Name the one of cold, foretold twofold:\"\n\n")

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
	text:cdprint(true, false, "A magic mouth on the wall speaks to you, saying \"")
	text:print("Name the endless byway and your path shall be guaranteed:\"\n")

	answer = text:readString()
	if (answer == "SINISTER") then
		local sq
		text:cdprint(false, true, "\n\"Right...\"")

		sq = self:getSqXY(6,1)
		sq.south.isWall = false
		sq.south.isDoor = true
		sq.south.gfx = "door"
		self.currentSquare.clearCode()
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
			text:cdprint(true, true, "%s has found a silver triangle.", c.name)
			text:clear()
		end
		self.currentSquare.clearCode()
	end
end

----------------------------------------
-- crystalStatue()
----------------------------------------
function dunSquares:crystalStatue()
	local inkey

	local function printIntro()
		bigpic:setBigpic("PIC_STATUE", "Golem!")
		text:cdprint(true, false, "A crystal statue comes to life before you, ready to attack! Will you:\n\nFight it")
		text:printExit()
	end

	printIntro()

	repeat
		inkey = getkey()

		if (inkey == "F") then
			if (battle:new("M_CRYSTAL_GOLEM", 1)) then
				if (party:isItemEquipped("Crystal Sword")) then
					self.currentSquare.clearCode()
					return
				end
				text:cdprint(false, true, "Something is happening to the golem's shattered body...")
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

function dunSquares:onyxKey()
end

function dunSquares:mouthFour()
end

function dunSquares:silverCircle()
end

function dunSquares:masterKey()
end

function dunSquares:sevenAnswers()
end

function dunSquares:largeCoffin()
end

function dunSquares:sleepingDragons()
end

function dunSquares:thorFigurine()
end

function dunSquares:swapDoors()
end

function dunSquares:silverDest()
end

function dunSquares:spectreSnare()
end

function dunSquares:boilingLiquid()
end

function dunSquares:mangarTreasure()
end

function dunSquares:mouthOne()
end

function dunSquares:possessCharacter()
end

function dunSquares:mangar()
end


























