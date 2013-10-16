----------------------------------------
-- Return structure
----------------------------------------
local buildingRval = {
	turnParty	= true,
	moveSquare	= true
}

----------------------------------------
-- takeStairs()
--
-- Common code for (T)aking the stairs
----------------------------------------
local function takeStairs(inString)
	local inkey

	text:cdprint(true, false, inString)
	text:printExit()
	repeat
		inkey = getkey()
		if (inkey == "T") then
			text:clear()
			return true
		end
	until (inkey == "E")

	return false
end

----------------------------------------
-- casino
----------------------------------------
local casino = {}
function casino:enter()
	text:csplash(true, true, "Lagoth Zanta has stolen all the casino's " ..
				"gold and put it out of business.")
	text:clear()
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
if false then
	if (rnd_and_x(3) == 0) then
		battle:random()
		if (globals.partyDied) then
			return
		end
	end
end

	self:resetBigpic()
	text:cdprint(true, false, "You're in an empty building.")
	text:printExit()
	getkey()

end

----------------------------------------
-- emptyhut
----------------------------------------
local emptyhut = building:new("Empty Hut", "PIC_EMPTYHUT")
function emptyhut:enter()
if false then
	if (rnd_and_x(3) == 0) then
		battle:random()
		if (globals.partyDied) then
			return true
		end
	end
end

	self:resetBigpic()
	text:cdprint(true, false, "You're in an empty hut.")
	text:printExit()
	getkey()
end

----------------------------------------
-- fanskar
----------------------------------------
local fanskar = {}
function fanskar:enter()
	local inkey

	bigpic:drawImage("PIC_EMPTYBLDG")
	if (not party:hasItem("Master Key")) then
		text:csplash(false, true,
			"You stand in the entry to Fanskar's fortress, but "..
			"the door is locked."
			)
		text:clear()
		return
	end

	text:cdprint(true, false,
		"You stand in the entry to Fanskar's fortress. A stairway " ..
		"leads to the main level.\n\nYou can:\n\n" ..
		"Take the stairway"
		)

	repeat
		text:printExit()
		inkey = getkey()

		if (inkey == "T") then
			currentLevel:enterDungeon("fort", 1);
			return
		end
	until (inkey == "E")
end

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
		buildingRval.moveSquare = false
		buildingRval.turnParty = false
		return
	end
	self:resetBigpic(true)
	text:csplash(true, true, "\nYou stand before a locked iron gate.")
	text:clear()

	buildingRval.turnParty = false
end

----------------------------------------
-- greycrypt
----------------------------------------
local greycrypt = building:new("Crypt", "PIC_EMPTYBLDG")
function greycrypt:enter()
	local inkey
	local answer

	self:resetBigpic()
	text:cdprint(true, false,
		"You stand in the entry to an ancient crypt. A passage "..
		"leads into the dark unknown.\n\nYou can:\n\nTake the passage"
		)

	repeat
		text:printExit()
		inkey = getkey()

		if (inkey == "T") then
			text:cdprint(true, false, 
				"A statue comes to life before you, and "..
				"says, \"Name this crypt to enter it.\"\n")
			answer = text:readString()
			if ((answer == "GREY") or (answer == "GREY CRYPT")) then
				currentLevel:enterDungeon("crypt", 1)
				text:clear()
				return	
			else
				text:csplash(true, true, "Wrong!!")
				text:clear()
				return
			end
		end
	until (inkey == "E")
end

----------------------------------------
-- doGuild
----------------------------------------
local doGuild = {}
function doGuild:enter()
	currentLevel.exit = true
	globals.gameState = globals.STATE_GUILD
	currentLevel = false

	buildingRval.turnParty = false
end

----------------------------------------
-- kazdek
----------------------------------------
local kazdek = building:new("Stone Man", "PIC_GOLEM")
function kazdek:enter()
	local inkey
	local answer
	local c

	self:resetBigpic()

	if (party:isItemEquipped("Item of K")) then
		text:csplash(true, true, 
			"You stand in Kazdek's hut. There is no one here."
			)
		text:clear()
		return
	end

	repeat
		text:cdprint(true, false,
			"\"Welcome, my friends,\" a stone man exclaims. " ..
			"\"My home is, of course, your home. Stay as long " ..
			"as you like.\" He falls asleep.\n\nYou can: \n" ..  
			"Speak to him"
		)
		text:printExit()
		inkey = getkey()

		if (inkey == "S") then
			text:cdprint(true, false, "What will you say?\n")
			answer = text:readString()

			if (answer ~= "KAZDEK") then
				text:csplash(true, true, "He does not awake.")
				text:clear()
			else
				text:csplash(true, true,
					"\"You called?\" he says, as he " ..
					"awakes. \"Oh, you would like an " ..
					"item of mine.\" He pulls something " ..
					"from the air. \"Take this.\" he " ..
					"then vanishes."
					)
				text:clear()
				c = party:giveItem("Item of K", true)
				if (not c) then
					text:cdprint(false, true,
						"But nobody has room for it!"
						)
					return
				else
					text:cdprint(false, true,
						"%s got it.", c.name
						)
					return 
				end
			end
		end
	until (inkey == "E")
end


local maze = {}

----------------------------------------
-- narn
----------------------------------------
local narn = building:new("Tmp. of Narn", "PIC_TEMPLEINT")
function narn:enter()
	local inkey
	self:resetBigpic()

if false then
	if (party:hasWinner()) then
		text:csplash(true, true,
			"\"Welcome, oh great Knight! Thy presence blesses "..
			"us all! Stay and rest as long as you desire.\""
			)
		text:clear()
		return
	end
end

	local function __printIntro()
		text:cdprint(true, false, 
			"You stand inside the Temple of Narn. There is a "..
			"large green altar before you.\n\nYou can:\n\n" ..
			"Approach the altar."
			)
		text:printExit()
	end

	__printIntro()
	repeat
		inkey = getkey()
		if (inkey == "A") then
			if (self:doApproach()) then
				return
			else
				__printIntro()
			end
		end
	until (inkey == "E")
end

----------------------------------------
-- narn:doApproach
----------------------------------------
function narn:doApproach()
	local c
	local i
	local segmentCount = 0

	text:cdprint(true, false, "Who will approach it?")
	c = party:readSlot()
	if (not c) then
		return false
	end

	for i = 1,7 do
		if (c:hasItem(string.format("Seg %d", i))) then
			segmentCount = segmentCount + 1
		end
	end

	if ((segmentCount == 0) or (c.class ~= "Archmage")) then
		text:csplash(true, true, "Nothing happened.")
		return true
	end

	if (segmentCount < 7) then
		text:csplash(true, true, 
			"...and yet not all things are complete. It felt as "..
			"if he was very close to great power..."
			)
		return
	end

	bigpic:drawImage("PIC_FORGESEGMENT")
	text:csplash(true, true, "Something strange is happening to him...")
	text:csplash(true, true, 
		"%s has become the One of Legend, known to all men as the "..
		"Destiny Knight! Power courses through him as never felt "..
		"before. The full extend of his powers is as yet unknown...",
		c.name
		)
	text:csplash(true, true,
		"...but they appear to be incredible! It was foretold that " ..
		"you shall receive power when the spirit has come upon you. "..
		"You must now go and be a witness of this to all...")
	
	text:clear()

	for i = 1,7 do
		c.inventory:dropItem(string.format("Seg %d", i))
	end

	c:giveItem("The Scepter", true, false)
	c.numAttacks = 8
	c.hasScepterMaybe = true

	return true
end

local stone = {}

----------------------------------------
-- tombs
----------------------------------------
local tombs = {}
function tombs:enter()
	bigpic:drawImage("PIC_TEMPLEINT")

	if (takeStairs(
		"You stand within the Temple of Darkness. A passage leads " ..
		"down to what probably are some very ancient tombs.\n\n"..
		"You can:\n\nTake the passage"
		)) then
		currentLevel:enterDungeon("tombs", 1)
	end
end

----------------------------------------
-- tower
----------------------------------------
local tower = {}
function tower:enter()
	bigpic:drawImage("PIC_EMPTYBLDG")
	
	if (takeStairs(
		"You stand in the entry to Dargoth's Tower. " ..
		"A stairway leads upward into the darkness.\n\n" ..
		"You can:\n\nTake the stairway"
		)) then
		currentLevel:enterDungeon("tower", 1)
	end
end

----------------------------------------
-- tree
----------------------------------------
local tree = {}
function tree:enter()
	text:cdprint(true, false, "You hit a tree!")

	buildingRval.turnParty = false
end

----------------------------------------
-- wall
----------------------------------------
local wall = {}
function wall:enter()
	text:cdprint(true, false, "You hit a city wall!")

	buildingRval.turnParty = false
end

----------------------------------------
-- cityBuildings table
----------------------------------------
cityBuildings = {
	casino		= casino,
	domain		= domain,
	empty		= empty,
	emptyhut	= emptyhut,
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
	buildingRval.turnParty	= true
	buildingRval.moveSquare	= true

	self[inBuilding]:enter(...)

	if (buildingRval.turnParty) then
		currentLevel:turnParty("back")
	end

	return buildingRval.moveSquare
end
