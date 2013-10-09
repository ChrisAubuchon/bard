----------------------------------------
-- casino
----------------------------------------
local casino = {}
function casino:enter()
	text:csplash(true, true, "Lagoth Zanta has stolen all the casino's " ..
				"gold and put it out of business.")
	text:clear()
	currentLevel:turnParty("back")
end


----------------------------------------
-- domain
----------------------------------------
local domain = {}
function domain:enter()
	local inkey

	bigpic:drawImage("PIC_DREN")

	repeat
		text:cdprint(true, false, 
			"\"Welcome, adventurers.\" The Magician to the " ..
			"King stands before you. \"I have a noble quest " ..
			"for you, if you'd hear the tale.\"\n\n" ..
			"You can:\n" ..
			"Listen to him\n" ..
			"Take the stairway"
			)
		text:printExit()
		inkey = getkey()

		if (inkey == "L") then
			text:csplash(true, true,
				"\"The story is a tragic one. The daughter " ..
				"of Endor the Mighty, King of all the land, "..
				"has been kidnapped by the Dark Lord, whose "..
				"domain lies just under this chamber."
				)
			text:csplash(true, true,
				"I would rescue her myself, but the Dark " ..
				"Lord has laid a mighty spell upon his " ..
				"domain and I am barred from ever entering. "..
				"You, however, can make it in."
				)
			text:csplash(true, true,
				"Take care, and be sure to map every inch "..
				"of the Dark One's domain. I'll be waiting "..
				"here for you to return.\""
				)
		elseif (inkey == "T") then
			city:enterDungeon("domain", 1)
			text:clear()
			return
		end
	until (inkey == "E")
end

----------------------------------------
-- Empty building
----------------------------------------
local empty = building:new("Empty Building", "PIC_EMPTYBLDG")
function empty:enter()
	if (rnd_and_x(3) == 0) then
		battle:random()
		if (globals.partyDied) then
			return
		end
	end

	self:resetBigpic()
	text:cdprint(true, false, "You're in an empty building.")
	text:printExit()
	getkey()
	currentLevel:turnParty("back")

	return true
end

----------------------------------------
-- emptyhut
----------------------------------------
local emptyhut = building:new("Empty Hut", "PIC_EMPTYHUT")
function emptyhut:enter()
	if (rnd_and_x(3) == 0) then
		battle:random()
		if (globals.partyDied) then
			return true
		end
	end

	self:resetBigpic()
	text:cdprint(true, false, "You're in an empty hut.")
	text:printExit()
	getkey()
	currentLevel:turnParty("back")

	return true
end

local entercity = {}
local enterwild = {}
function enterwild:enter(...)
	local x
	local y
	local dir = currentLevel.direction

	x, y = ...
	currentLevel.exit = true
	currentLevel.exitLocation = "wild"
	currentLevel = wild:new()
	currentLevel:enter(x, y, dir)
end

----------------------------------------
-- fanskar
----------------------------------------
local fanskar = {}

----------------------------------------
-- fort
----------------------------------------
local fort = {}

----------------------------------------
-- gate
----------------------------------------
local gate = building:new("Iron Gate", "PIC_IRONGATE")
function gate:enter()
	if (party:isItemEquipped("Master Key")) then
		return false
	end

	self:resetBigpic(true)
	text:csplash(true, true, "\nYou stand before a locked iron gate.")
	text:clear()
	currentLevel.currentSquare = currentLevel.previousSquare
	currentLevel:turnParty("back")
end

----------------------------------------
-- greycrypt
----------------------------------------
local greycrypt = {}

----------------------------------------
-- doGuild
----------------------------------------
local doGuild = {}
function doGuild:enter()
	currentLevel.exit = true
	globals.gameState = globals.STATE_GUILD
	currentLevel = false
end

local kazdek = {}
local maze = {}
local narn = {}

----------------------------------------
-- sagehut
----------------------------------------
local sagehut = building:new("The Sage", "PIC_SAGE")
function sagehut:enter()
--	if (party:hasWinner()) then
--		bigpic:setBigpic("Empty Hut", "PIC_EMPTYHUT")
--		text:csplash(true, true, "\nYou're in an empty hut.")
--		text:clear()
--	elseif (party:hasItem("The Scepter")) then
--		self:resetBigpic()
--		text:csplash(true, true,
--			"\"Welcome, my friends, to the home of Lagoth " ..
--			"Zanta! You have been cunning, it is true, " ..
--			"buy your days have now come to an end. Your " ..
--			"quest is through, fools. Prepare to die.\""
--			)
--		if (not battle:new("Balder Guard", 16, "Balder Guard", 16, 
--				"Lagoth Zanta", 1)) then
--			if (globals.partyDied) then
--				return false
--			end
--			currentLevel:turnParty("back")
--			return true
--		else
--			local xxx_add_one_million_xp_to_party
--			local xxx_set_hasWon_flag_on_party
--
--			bigpic:drawImage("PIC_VICTORY")
--			text:csplash(true, true, 
--				"Well done! At the death of Zanta the armies "..
--				"of the Realm and march to defeat the " ..
--				"invading hordes, who will easily fall. " ..
--				"The King himself enters the hut and salutes "..
--				"you."
--				)
--			text:csplash(true, true,
--				"\"Oh mighty ones, I thank thee truly for " ..
--				"they service to the realm. May the All High "..
--				"bless thee and keep thee strong and "..
--				"prosperous. In my own little way, I give "..
--				"you all a little bonus."
--			)
--			text:csplash(true, true,
--				"Congratulations, your quest is over. But a " ..
--				"new one is bound to begin..."
--				)
--			text:clear()
--			self.exit = true
--			globals.gameState = globals.STATE_GUILD
--			return true
--		end
--	else
		return self:askSage()
--	end
end

function sagehut:askSage()
	local inkey

	self:resetBigpic()
	repeat
		text:cdprint(true, false, 
			"The Sage says, \"Gold can buy thee the wisdom " ..
			"of the ancients, my friends.\"" ..
			"\n\nYou can:\n\n" ..
			"(T)alk to the Sage\n" ..
			"(E)xit his hut"
			)
		inkey = getkey()

		if (inkey == "T") then
			self:talkToSage()
		end
	until (inkey == "E")

	return true
end

function sagehut:talkToSage()
	local inkey
	local c
	local answer

	text:cdprint(true, false, "Who will talk to the Sage?")
	c = party:readSlot()
	if (not c) then
		return
	end

	text:cdprint(true, false, "What will you ask him about?\n")
	answer = text:readString()
	if (not answer) then
		return
	end

	answer = string.gsub(answer, "THE ", "")
end

local stone = {}
local tombs = {}
local tower = {}

----------------------------------------
-- tree
----------------------------------------
local tree = {}
function tree:enter()
	text:cdprint(true, false, "You hit a tree!")
	currentLevel:turnParty("back")
	return true
end

----------------------------------------
-- wall
----------------------------------------
local wall = {}
function wall:enter()
	text:cdprint(true, false, "You hit a city wall!")
	currentLevel:turnParty("back")
	return true
end

----------------------------------------
-- cityBuildings table
----------------------------------------
cityBuildings = {
	casino		= casino,
	domain		= domain,
	empty		= empty,
	emptyhut	= emptyhut,
	entercity	= entercity,
	enterwild	= enterwild,
	fanskar		= fanskar,
	fort		= fort,
	gate		= gate,
	greycrypt	= greycrypt,
	guild		= doGuild,
	kazdek		= kazdek,
	maze		= maze,
	narn		= narn,
	sagehut		= sagehut,
	stone		= stone,
	tombs		= tombs,
	tower		= tower,
	tree		= tree,
	wall		= wall
}

function cityBuildings:enter(inBuilding, ...)
	return self[inBuilding]:enter(...)
end
