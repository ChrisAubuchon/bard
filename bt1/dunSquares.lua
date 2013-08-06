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

	



























