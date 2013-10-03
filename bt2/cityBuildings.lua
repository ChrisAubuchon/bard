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
end

----------------------------------------
-- emptyhut
----------------------------------------
local emptyhut = {}

local entercity = {}
local enterwild = {}
function enterwild:enter(...)
	local x
	local y

	x, y = ...
	dprint("Entering wild (%d,%d)", x, y)
	currentLevel.currentSquare = currentLevel.previousSquare
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
local sagehut = {}
local stone = {}
local tombs = {}
local tower = {}
local tree = {}
local wall = {}

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
