----------------------------------------
-- City gates
----------------------------------------
local cityGates = {}

btTable.addParent(cityGates, building:new("City Gate", "PIC_CITYGATE"))
btTable.setClassMetatable(cityGates)

function cityGates:enter()
	self:resetBigpic(true)
	text:cdprint(true, false, "You stand before the city gates, which are blocked by a gigantic snow drift.")
	text:printContinue()
	getkey()

	currentLevel:turnParty("back")
end

----------------------------------------
-- doGuild
----------------------------------------
local doGuild = {}
function doGuild:enter()
	currentLevel.exit = true
	globals.gameState = globals.STATE_GUILD
	currentLevel = false
end

----------------------------------------
-- Empty building
----------------------------------------
local empty = {}

btTable.addParent(empty, building:new("Empty Building", "PIC_EMPTYBLDG"))
btTable.setClassMetatable(empty)

function empty:enter()
	if (rnd_xdy(1,4) == 1) then
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
-- Harkyn entry square
----------------------------------------
local harkyn = {}

btTable.addParent(harkyn, building:new("Castle", "PIC_HARKYNCASTLE"))
btTable.setClassMetatable(harkyn)

function harkyn:enter()
	local inkey

	self:resetBigpic(true)
	text:cdprint(true, false, "This is the entry chamber to Harkyn's ")
	text:print("Castle. It is not guarded, but a sign threatens ")
	text:print("trespassers with death. You can:\n\nTake stairs up")
	text:printExit()

	repeat
		inkey = getkey()
		if (inkey == "T") then
			text:clear()
			city:enterDungeon("castle", 1)
			return
		end
	until (inkey == "E")

	currentLevel:turnParty("back")
end

----------------------------------------
-- Kylearan entry square
----------------------------------------
local kylearan = {}

btTable.addParent(kylearan, building:new("Amber Tower", "PIC_TOWERINT"))
btTable.setClassMetatable(kylearan)

function kylearan:enter()
	local inkey

	self:resetBigpic()
	text:cdprint(true, false, "This is the entry chamber to Kylearan's ")
	text:print("Amber Tower. A stairwell leads up to a lofty level of ")
	text:print("chambers. You can:\n\nTake stairs")
	text:printExit()

	repeat
		inkey = getkey()
		if (inkey == "T") then
			text:clear()
			city:enterDungeon("kylr", 1)
			return
		end
	until (inkey == "E") 
	currentLevel:turnParty("back")
end

----------------------------------------
-- madgod temple code
----------------------------------------
local madgod = {}

btTable.addParent(madgod, building:new("Mad God", "PIC_TEMPLEINT"))
btTable.setClassMetatable(madgod)

function madgod:enter()
	local inkey

	city:enterDungeon("cellars", 1)
	local xxx_uncomment_to_get_madgod_code = true
if false then
	self:resetBigpic()
	repeat
		text:cdprint(true, false, "\nThis is the temple of the Mad God. What is thy business, unbeliever?\n\nSpeak to priest")
		text:printExit()

		inkey = getkey()
		if ((inkey > "0") and (inkey < "7")) then
			local c = party:readMember(inkey)
			if (c) then
				c:printCharacter()
				self:resetBigpic()
			end
		elseif (inkey == "S") then
			text:cdprint(true, false, "\nOnly those who know the name of the Mad One are welcome.\nWhat wilt thou say?\n\n")
			local inString = text:readString()
			if (inString ~= "TARJAN") then
				text:cdprint(false, true, "\n\n\"Quit thy babbling,\" the priest says.")
			else
				text:cdprint(false, true, "\n\"Speak not the name of the High One so loudly, lest he awaken,\" the priest says. \"Enter the catacombs, believer.\"")
				city:enterDungeon("catacombs", 1)
				return
			end
		end
	
	until (inkey == "E")

	currentLevel:turnParty("back")
end
end

----------------------------------------
-- mangar entry square
----------------------------------------
local mangar = {}

btTable.addParent(mangar, building:new("The Tower", "PIC_TOWERINT"))
btTable.setClassMetatable(mangar)

function mangar:enter()
	local inkey

	text:clear()

	-- Party must have the Onyx Key equipped to enter
	--
	if (not party:isItemEquipped("Onyx Key")) then
		bigpic:drawImage("PIC_MAGICMOUTH")
		bigpic:setTitle("Magic Mouth")
		text:splashMessage("A magic mouth on the wall speaks to you: \"Despised ones, none save Mangar may enter his demesne.")
		currentLevel:turnParty("back")
		return
	end

	self:resetBigpic()
	text:print("This is the entry chamber to Mangar's Tower. A stairwell ")
	text:print("up to the first level of traps and terrors. You can:\n\n")
	text:print("Take stairs")
	text:printExit()

	repeat
		inkey = getkey()

		if (inkey == "T") then
			text:clear()
			city:enterDungeon("mangar", 1)
			return	
		end
	until (inkey == "E")

	currentLevel:turnParty("back")
end

----------------------------------------
-- Sewer Portal code
----------------------------------------
local sewerPortal = {}
function sewerPortal:enter()
	bigpic:setTitle("Portal...")
	text:cdprint(true, false, "There is an entrance to the city sewers ")
	text:print("here. do you wish to take it?")
	if (text:getYesNo()) then	
		text:clear()
		city:enterDungeon("cellars", 4)
		currentLevel.currentSquare = currentLevel:getSqXY(17, 16)
		globals.citySquare = city:getLabelXY(27, 5)
		return
	end
end

----------------------------------------
-- stables
----------------------------------------
local stables = {}
btTable.addParent(stables, building:new("Empty Building", "PIC_EMPTYBLDG"))
btTable.setClassMetatable(stables)

function stables:enter()
	self:resetBigpic()
	text:cdprint(true, false, "\nSorry, friends, all the horses have been eaten by creatures!")
	text:printExit()
	getkey()
	currentLevel:turnParty("back")
end

----------------------------------------
-- cityBuildings table
----------------------------------------
cityBuildings = {
	cityGates	= cityGates,
	empty		= empty,
	guild		= doGuild,
	harkyn		= harkyn,
	kylearan	= kylearan,
	madgod		= madgod,
	mangar		= mangar,
	sewerPortal	= sewerPortal,
	stables		= stables
}

function cityBuildings:enter(inBuilding)
	self[inBuilding]:enter()
end
